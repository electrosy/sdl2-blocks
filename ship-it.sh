#!/bin/bash
# ship-it.sh — place this in the sdl2-blocks root directory
# Usage: ./ship-it.sh [optional-label]
# Example: ./ship-it.sh may-27-2026
# Produces: ../ablockalypse-may-27-2026-a.zip (or -b, -c, etc. if prior ones exist)

set -e

LABEL=${1:-$(date +%b-%d-%Y | tr '[:upper:]' '[:lower:]')}

# Find the next available letter suffix
for i in {a..z}; do
    OUTPUT="../ablockalypse-${LABEL}-${i}.zip"
    if [[ ! -f "$OUTPUT" ]]; then
        break
    fi
done

zip -r "$OUTPUT" . \
  --exclude ".git/*" \
  --exclude "build/*" \
  --exclude "*.o" \
  --exclude "*.d" \
  --exclude "ship-it.sh"

# ── Strip ELF executables ────────────────────────────────────────────────────
# zip --exclude can't filter by file content, so we post-process.
# Extract each file, check with `file`, remove any ELF binary from the zip.
echo "Scanning for ELF executables..."
TMPDIR=$(mktemp -d)
zipinfo -1 "$OUTPUT" | while IFS= read -r entry; do
    # Skip directories
    [[ "$entry" == */ ]] && continue
    # Extract just this file to a temp location
    unzip -q -o "$OUTPUT" "$entry" -d "$TMPDIR" 2>/dev/null || continue
    extracted="$TMPDIR/$entry"
    if file "$extracted" 2>/dev/null | grep -qE 'ELF.*executable'; then
        zip -d "$OUTPUT" "$entry" > /dev/null
        echo "  removed ELF: $entry"
    fi
done
rm -rf "$TMPDIR"

echo "✓ Created: $OUTPUT"
