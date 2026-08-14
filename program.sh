#!/usr/bin/env sh
set -eu

OPENOCD_SCRIPTS=/usr/share/openocd/scripts
FIRMWARE=build/gd32f103rct6.bin

test -f "$FIRMWARE" || {
    echo "Firmware not found: $FIRMWARE" >&2
    exit 1
}

exec openocd -s "$OPENOCD_SCRIPTS" \
    -f interface/stlink-v2.cfg \
    -f target/stm32f1x.cfg \
    -c "program $FIRMWARE 0x08000000 verify reset exit"

# Debug
# exec openocd -s "$OPENOCD_SCRIPTS" \
#     -f interface/stlink-v2.cfg \
#     -f target/stm32f1x.cfg 

