#!/bin/sh
set -eu

monitor=src/monitor.sh

grep -Fq 'map_controller "$intel" 5 6' "$monitor"
grep -Fq 'map_controller "$asm"   1 2 3 4' "$monitor"

if grep -Fq 'map_controller "$intel" 3 4' "$monitor" ||
   grep -Fq 'map_controller "$asm"   1 2 5 6' "$monitor"; then
  echo "legacy Pro-oriented bay mapping is still present" >&2
  exit 1
fi

echo "monitor bay mapping tests passed"
