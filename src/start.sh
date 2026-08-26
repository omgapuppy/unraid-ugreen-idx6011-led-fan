#!/bin/bash
set -u
NAME=unraid-ugreen-idx6011-led-fan
PLG=/boot/config/plugins/$NAME
PRODUCT=$(tr -d '\r\n' </sys/class/dmi/id/product_name 2>/dev/null)
VENDOR=$(tr -d '\r\n' </sys/class/dmi/id/sys_vendor 2>/dev/null)
[ "$VENDOR" = UGREEN ] || { logger -t ugreen-idx "unsupported DMI vendor: $VENDOR"; exit 0; }
case "$PRODUCT" in iDX6011|iDX6012) ;; *) logger -t ugreen-idx "unsupported DMI product: $PRODUCT"; exit 0;; esac
# The package glob is intentionally passed through unchanged to installpkg.
# shellcheck disable=SC2144
command -v i2cset >/dev/null 2>&1 || { [ -f "$PLG"/i2c-tools-*.txz ] && installpkg "$PLG"/i2c-tools-*.txz >/dev/null 2>&1; }
modprobe i2c-dev 2>/dev/null
modprobe -r led_ugreen 2>/dev/null
[ -x "$PLG/ugreen_leds_cli" ] || { logger -t ugreen-idx "LED CLI missing"; exit 1; }
install -m0755 "$PLG/ugreen_leds_cli" /usr/local/bin/ugreen_leds_cli
install -m0755 "$PLG/monitor.sh" /usr/local/bin/ugreen-idx6011-monitor
[ -x "$PLG/idx6011-fan-status" ] && install -m0755 "$PLG/idx6011-fan-status" /usr/local/bin/idx6011-fan-status
[ -f /run/ugreen-leds.pid ] && kill "$(cat /run/ugreen-leds.pid)" 2>/dev/null
sleep 1
( setsid /usr/local/bin/ugreen-idx6011-monitor >/var/log/ugreen-leds-monitor.log 2>&1 </dev/null & )
logger -t ugreen-idx "LED monitor started for $PRODUCT"
