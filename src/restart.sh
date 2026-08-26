#!/bin/bash
P=/boot/config/plugins/unraid-ugreen-idx6011-led-fan
[ -f "$P/stop.sh" ] && bash "$P/stop.sh"
[ -f "$P/start.sh" ] && bash "$P/start.sh"
