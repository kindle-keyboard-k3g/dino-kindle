#!/bin/sh
# Kindle Keyboard (K3) Dino Launcher Script

set -e

cleanup() {
    echo "Restoring Kindle environment..."
    # Clear framebuffer
    dd if=/dev/zero of=/dev/fb0 bs=240000 count=1 2>/dev/null || true
    # Restart Kindle Java UI framework
    /etc/init.d/framework start || true
    echo "Done."
}

trap cleanup EXIT INT TERM

echo "Preparing Kindle environment for Chrome Dino..."
# Defer system suspend
lipc-set-prop com.lab126.powerd -i deferSuspend 1 2>/dev/null || true

# Stop heavy Java framework to reclaim >120MB RAM and isolate CPU
/etc/init.d/framework stop || true
sleep 1

# Work around potential VFAT noexec mount on /mnt/us
TARGET_BIN="/mnt/us/dino/bin/dino"
RUN_BIN="/tmp/dino"

if [ -f "$TARGET_BIN" ]; then
    cp -f "$TARGET_BIN" "$RUN_BIN"
elif [ -f "./bin/dino-arm32" ]; then
    cp -f "./bin/dino-arm32" "$RUN_BIN"
elif [ -f "./bin/dino" ]; then
    cp -f "./bin/dino" "$RUN_BIN"
fi

chmod +x "$RUN_BIN"

echo "Starting Dino..."
"$RUN_BIN" "$@" || true
