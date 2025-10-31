#!/usr/bin/env python3
"""
Fix Arduino_GFX library for ESP32 Arduino Core 3.x compatibility
Adds defined() wrapper around CONFIG_IDF_TARGET_* macros
"""

import os
import re
from pathlib import Path

ARDUINO_GFX_DIR = Path.home() / "Documents/Arduino/libraries/Arduino_GFX"

def fix_file(filepath):
    """Fix CONFIG_IDF_TARGET macros in a single file"""
    try:
        with open(filepath, 'r', encoding='utf-8', errors='ignore') as f:
            content = f.read()

        original_content = content

        # Fix #if CONFIG_IDF_TARGET -> #if defined(CONFIG_IDF_TARGET)
        content = re.sub(
            r'#if (CONFIG_IDF_TARGET_\w+)',
            r'#if defined(\1)',
            content
        )

        # Fix #elif CONFIG_IDF_TARGET -> #elif defined(CONFIG_IDF_TARGET)
        content = re.sub(
            r'#elif (CONFIG_IDF_TARGET_\w+)',
            r'#elif defined(\1)',
            content
        )

        # Fix || CONFIG_IDF_TARGET -> || defined(CONFIG_IDF_TARGET)
        content = re.sub(
            r'\|\| (CONFIG_IDF_TARGET_\w+)',
            r'|| defined(\1)',
            content
        )

        # Fix (CONFIG_IDF_TARGET at start -> (defined(CONFIG_IDF_TARGET
        content = re.sub(
            r'\((CONFIG_IDF_TARGET_\w+) \|\|',
            r'(defined(\1) ||',
            content
        )

        # Only write if changed
        if content != original_content:
            # Backup original
            with open(str(filepath) + '.bak', 'w', encoding='utf-8') as f:
                f.write(original_content)

            # Write fixed version
            with open(filepath, 'w', encoding='utf-8') as f:
                f.write(content)

            return True
        return False

    except Exception as e:
        print(f"Error processing {filepath}: {e}")
        return False

def main():
    print("Fixing Arduino_GFX library for ESP32 Core 3.x...")
    print(f"Library path: {ARDUINO_GFX_DIR}")

    if not ARDUINO_GFX_DIR.exists():
        print(f"ERROR: Arduino_GFX not found at {ARDUINO_GFX_DIR}")
        return 1

    fixed_count = 0

    # Fix all .h and .cpp files in src directory
    for filepath in ARDUINO_GFX_DIR.glob("src/**/*"):
        if filepath.suffix in ['.h', '.cpp']:
            if fix_file(filepath):
                print(f"Fixed: {filepath.name}")
                fixed_count += 1

    print(f"\nDone! Fixed {fixed_count} files")
    print("Backup files saved with .bak extension")
    return 0

if __name__ == '__main__':
    exit(main())
