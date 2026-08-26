#!/bin/bash
set -euo pipefail
cd "$(dirname "$0")/.."
VER=${1:?usage: mkpayload.sh VERSION}
NAME=unraid-ugreen-idx6011-led-fan
STAGE=$(mktemp -d)
trap 'rm -rf "$STAGE"' EXIT
install -m0755 src/start.sh src/stop.sh src/restart.sh src/monitor.sh src/calibrate.sh "$STAGE/"
install -m0755 prebuilt/ugreen_leds_cli "$STAGE/"
[ -x idx6011-fan-status ] && install -m0755 idx6011-fan-status "$STAGE/"
install -m0644 prebuilt/i2c-tools-*.txz "$STAGE/"
install -m0644 src/UgreenIDX6011Control.page "$STAGE/"
install -m0644 src/webgui/idx-inc.php src/webgui/idx.js "$STAGE/"
install -m0644 src/webgui/plugin-desc.md "$STAGE/README.md"
mkdir -p release
(cd "$STAGE" && find . -type f | sort | tar cJf - -T -) > "release/$NAME-$VER.txz"
if command -v sha256sum >/dev/null; then sha256sum "release/$NAME-$VER.txz" | awk '{print $1}'; else shasum -a 256 "release/$NAME-$VER.txz" | awk '{print $1}'; fi
