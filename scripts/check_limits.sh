#!/usr/bin/env bash
set -e

DIR="$(cd "$(dirname "$0")/.." && pwd)"
echo "Auditing Clean Code & Object Calisthenics Limits in $DIR/src..."

VIOLATIONS=0

# Check file length (< 200 lines)
while IFS= read -r file; do
    lines=$(wc -l < "$file")
    if [ "$lines" -ge 200 ]; then
        echo "FAIL: File exceeds 200 lines ($lines lines): $file"
        VIOLATIONS=$((VIOLATIONS + 1))
    fi
done < <(find "$DIR/src" -type f \( -name "*.h" -o -name "*.cpp" \))

# Check for raw else statements in domain and game logic
while IFS= read -r file; do
    if grep -nE "\belse\b" "$file" > /dev/null; then
        echo "WARNING/CHECK: 'else' keyword found in $file:"
        grep -nE "\belse\b" "$file" || true
    fi
done < <(find "$DIR/src/domain" "$DIR/src/game" -type f \( -name "*.h" -o -name "*.cpp" \))

if [ "$VIOLATIONS" -eq 0 ]; then
    echo "SUCCESS: All files strictly comply with the <200 lines limit!"
    exit 0
else
    echo "FAILED: $VIOLATIONS file length violations detected."
    exit 1
fi
