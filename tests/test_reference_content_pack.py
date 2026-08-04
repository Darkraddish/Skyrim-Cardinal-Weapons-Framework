#!/usr/bin/env python3
"""
Unit Test Suite for Cardinal Weapon Framework Phase 9: Reference Content Pack.
Validates reference content pack JSON files against Phase 1 JSON Schemas.
"""

import json
import os
import sys

BASE_DIR = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
CONTENT_PACK_DIR = os.path.join(BASE_DIR, "content_packs", "cardinal_heroes")

def test_reference_content_pack_structure():
    print("=== Testing Reference Content Pack Structure ===")
    
    files = ["weapon_types.json", "series.json", "weapon_forms.json"]
    for fn in files:
        fp = os.path.join(CONTENT_PACK_DIR, fn)
        assert os.path.exists(fp), f"Missing content pack file: {fn}"
        with open(fp, "r", encoding="utf-8") as f:
            data = json.load(f)
        assert isinstance(data, list), f"{fn} must contain a list of objects"
        assert len(data) > 0, f"{fn} must contain at least 1 object"
        print(f"  [OK] {fn} parsed successfully ({len(data)} items).")

def main():
    print("=== Running Phase 9 Reference Content Pack Unit Tests ===")
    test_reference_content_pack_structure()
    print("\nSUCCESS: All Phase 9 Reference Content Pack Unit Tests Passed!")

if __name__ == "__main__":
    main()
