#!/bin/bash
BIN=/usr/local/bin/ugreen_leds_cli
[ -f /run/ugreen-leds.pid ] && kill "$(cat /run/ugreen-leds.pid)" 2>/dev/null
# The monitor owns the non-Pro SMBus register layout; stopping it is sufficient.
rm -f /run/ugreen-leds.pid /usr/local/bin/ugreen-idx6011-monitor /usr/local/bin/ugreen_leds_cli /usr/local/bin/idx6011-fan-status
logger -t ugreen-idx "LED monitor stopped"
