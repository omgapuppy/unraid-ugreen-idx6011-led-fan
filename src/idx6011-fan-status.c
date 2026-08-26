/*
 * Read-only tachometer probe for the non-Pro UGREEN iDX6011/iDX6012.
 *
 * The ITE IT55xx EC is accessed through the conventional ACPI EC ports.  This
 * program deliberately contains no EC write operation: it is safe to use for
 * hardware discovery before enabling any fan-control daemon.
 *
 * Output is line-oriented key=value text, for example:
 *   status=ok model=iDX6011
 *   fan=sysfan1 tach_hi=0x96 tach_lo=0x97 rpm=618
 */

#define _DEFAULT_SOURCE

#include <errno.h>
#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <sys/file.h>
#include <sys/io.h>
#include <sys/stat.h>
#include <unistd.h>

#define EC_DATA_PORT 0x62
#define EC_CMD_PORT  0x66
#define EC_READ_CMD  0x80
#define EC_IBF       0x02
#define EC_OBF       0x01

#define EC_WAIT_LOOPS 20000U
#define EC_WAIT_US    50U

struct fan_probe {
    const char *name;
    unsigned tach_hi;
    unsigned tach_lo;
};

static const struct fan_probe fans[] = {
    { "sysfan1", 0x96, 0x97 },
    { "sysfan2", 0x98, 0x99 },
};

static int read_trimmed(const char *path, char *dst, size_t dst_size)
{
    FILE *file;
    size_t len;

    if (dst_size < 2)
        return -1;
    file = fopen(path, "r");
    if (!file)
        return -1;
    if (!fgets(dst, (int)dst_size, file)) {
        fclose(file);
        return -1;
    }
    fclose(file);

    len = strcspn(dst, "\r\n");
    dst[len] = '\0';
    return *dst ? 0 : -1;
}

static int supported_dmi(char model[64])
{
    char vendor[64];

    if (read_trimmed("/sys/class/dmi/id/sys_vendor", vendor, sizeof vendor) != 0 ||
        read_trimmed("/sys/class/dmi/id/product_name", model, 64) != 0)
        return -1;

    if (strcmp(vendor, "UGREEN") != 0)
        return 0;
    if (strcmp(model, "iDX6011") == 0 || strcmp(model, "iDX6012") == 0)
        return 1;
    return 0;
}

static int wait_ibf_clear(void)
{
    unsigned i;

    for (i = 0; i < EC_WAIT_LOOPS; i++) {
        if ((inb(EC_CMD_PORT) & EC_IBF) == 0)
            return 0;
        usleep(EC_WAIT_US);
    }
    return -1;
}

static int wait_obf_set(void)
{
    unsigned i;

    for (i = 0; i < EC_WAIT_LOOPS; i++) {
        if (inb(EC_CMD_PORT) & EC_OBF)
            return 0;
        usleep(EC_WAIT_US);
    }
    return -1;
}

static int ec_read_byte(unsigned address, uint8_t *value)
{
    if (wait_ibf_clear() != 0)
        return -1;
    /* Read command and address are the only EC transactions in this program. */
    outb(EC_READ_CMD, EC_CMD_PORT);
    if (wait_ibf_clear() != 0)
        return -1;
    outb((uint8_t)address, EC_DATA_PORT);
    if (wait_obf_set() != 0)
        return -1;
    *value = inb(EC_DATA_PORT);
    return 0;
}

static int read_fan_rpm(const struct fan_probe *fan, unsigned *rpm)
{
    uint8_t hi, lo;

    if (ec_read_byte(fan->tach_hi, &hi) != 0 ||
        ec_read_byte(fan->tach_lo, &lo) != 0)
        return -1;
    *rpm = ((unsigned)hi << 8) | lo;
    return 0;
}

static int acquire_ec_lock(void)
{
    int fd;

    (void)mkdir("/run/lock", 0755);
    fd = open("/run/lock/ugreen-idx6011-ec.lock", O_CREAT | O_RDWR, 0644);
    if (fd < 0)
        return -1;
    if (flock(fd, LOCK_EX) != 0) {
        close(fd);
        return -1;
    }
    return fd;
}

int main(void)
{
    char model[64] = "";
    int dmi;
    int lock_fd = -1;
    int io_enabled = 0;
    size_t i;

    dmi = supported_dmi(model);
    if (dmi < 0) {
        printf("status=error reason=dmi_unavailable\n");
        return 2;
    }
    if (dmi == 0) {
        printf("status=unsupported model=%s\n", model[0] ? model : "unknown");
        return 2;
    }

    lock_fd = acquire_ec_lock();
    if (lock_fd < 0) {
        printf("status=error reason=ec_lock errno=%d\n", errno);
        return 3;
    }

    if (ioperm(EC_DATA_PORT, 1, 1) != 0 || ioperm(EC_CMD_PORT, 1, 1) != 0) {
        printf("status=error reason=ioperm errno=%d\n", errno);
        (void)flock(lock_fd, LOCK_UN);
        close(lock_fd);
        return 3;
    }
    io_enabled = 1;

    printf("status=ok model=%s\n", model);
    for (i = 0; i < sizeof fans / sizeof fans[0]; i++) {
        unsigned rpm;
        if (read_fan_rpm(&fans[i], &rpm) != 0) {
            printf("status=error fan=%s reason=ec_read\n", fans[i].name);
            if (io_enabled) {
                (void)ioperm(EC_DATA_PORT, 1, 0);
                (void)ioperm(EC_CMD_PORT, 1, 0);
            }
            (void)flock(lock_fd, LOCK_UN);
            close(lock_fd);
            return 3;
        }
        printf("fan=%s tach_hi=0x%02x tach_lo=0x%02x rpm=%u\n",
               fans[i].name, fans[i].tach_hi, fans[i].tach_lo, rpm);
    }

    if (io_enabled) {
        (void)ioperm(EC_DATA_PORT, 1, 0);
        (void)ioperm(EC_CMD_PORT, 1, 0);
    }
    (void)flock(lock_fd, LOCK_UN);
    close(lock_fd);
    return 0;
}
