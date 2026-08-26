#!/bin/sh
set -eu

src=src/idx6011-fan-status.c

# EC reads necessarily write the EC read command (0x80) and address to the
# controller ports.  They must never issue the EC write command or fan-duty
# writes.
if grep -Eq '0x81|ec_write|FAN_DUTY' "$src"; then
    echo "FAIL: EC write command or fan-duty path found" >&2
    exit 1
fi

grep -F 'strcmp(model, "iDX6011")' "$src" >/dev/null
grep -F 'strcmp(model, "iDX6012")' "$src" >/dev/null
grep -F '{ "sysfan1", 0x96, 0x97 }' "$src" >/dev/null
grep -F '{ "sysfan2", 0x98, 0x99 }' "$src" >/dev/null
grep -F 'status=ok model=' "$src" >/dev/null
grep -F 'rpm=%u' "$src" >/dev/null

echo "OK: read-only DMI-gated non-Pro fan probe checks passed"
