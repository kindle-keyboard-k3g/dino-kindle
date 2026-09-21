#!/bin/sh
set -e

DIR="$(cd "$(dirname "$0")/.." && pwd)"
PACKAGE_DIR="$DIR/tmp/package/dino"

echo "Packaging Kindle Keyboard Dino distribution..."
rm -rf "$DIR/tmp/package"
mkdir -p "$PACKAGE_DIR/bin"

if [ ! -f "$DIR/bin/dino-arm32" ]; then
    echo "Building ARMv6 binary first..."
    make -C "$DIR" kindle
fi

cp "$DIR/bin/dino-arm32" "$PACKAGE_DIR/bin/dino"
cp "$DIR/scripts/launch_kindle.sh" "$PACKAGE_DIR/launch.sh"
chmod +x "$PACKAGE_DIR/launch.sh" "$PACKAGE_DIR/bin/dino"

cat << 'EOF' > "$PACKAGE_DIR/README.txt"
Chrome Offline Dino for Kindle Keyboard (K3 / K3G / K3W)
=========================================================

Installation:
1. Connect Kindle Keyboard to computer via USB.
2. Copy the entire 'dino' folder into the root of Kindle user storage:
   /mnt/us/dino/
3. Eject USB safely.
4. Open your SSH or terminal session on the Kindle and run:
   /mnt/us/dino/launch.sh

Controls:
- Jump: Spacebar, 5-Way Up, 5-Way Center Click, Top Page Turn Button (<)
- Duck: Down Arrow, 'D' Key, Bottom Page Turn Button (>)
- Restart: Spacebar or 5-Way Up (when Game Over)
- Quit: Back Button, Escape, or 'Q' Key
EOF

TARBALL="$DIR/bin/dino-k3-arm32.tar.gz"
tar -czf "$TARBALL" -C "$DIR/tmp/package" dino

echo "Package successfully generated at: $TARBALL"
ls -lh "$TARBALL"
