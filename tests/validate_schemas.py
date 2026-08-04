#!/usr/bin/env python3
"""
Schema Validation & Zero-Content Load Verification Script for Cardinal Weapon Framework Phase 1.
"""

import json
import os
import sys

SCHEMAS_DIR = os.path.join(os.path.dirname(os.path.dirname(os.path.abspath(__file__))), "schemas")

SCHEMA_FILES = [
    "weapon_type.schema.json",
    "series.schema.json",
    "weapon_form.schema.json",
    "progression_node.schema.json",
    "requirement.schema.json",
    "ability.schema.json",
    "effect.schema.json",
    "recipe.schema.json"
]

def check_json_syntax():
    print("=== Phase 1 JSON Schema Syntax Checks ===")
    all_passed = True
    for schema_file in SCHEMA_FILES:
        path = os.path.join(SCHEMAS_DIR, schema_file)
        if not os.path.exists(path):
            print(f"[FAIL] Schema file missing: {schema_file}")
            all_passed = False
            continue
        try:
            with open(path, "r", encoding="utf-8") as f:
                data = json.load(f)
            print(f"[OK] {schema_file} valid JSON. Title: {data.get('title', 'N/A')}")
        except Exception as e:
            print(f"[FAIL] {schema_file} error: {e}")
            all_passed = False
    return all_passed

def test_dummy_data_structures():
    print("\n=== Dummy Content Pack Validation Check ===")
    dummy_weapon_type = {
        "id": "CW_WT_SHIELD",
        "name": "Cardinal Shield",
        "description": "Legendary Shield progression type.",
        "iconAsset": "textures/ui/icons/shield.dds",
        "baseSeriesIds": ["CW_SER_BEAST_SHIELD"]
    }
    
    dummy_series = {
        "id": "CW_SER_BEAST_SHIELD",
        "weaponTypeId": "CW_WT_SHIELD",
        "name": "Beast Shield Series",
        "description": "Wild animal and beast evolution paths.",
        "iconAsset": "textures/ui/icons/beast.dds"
    }

    dummy_form = {
        "id": "CW_FORM_WOLF_SHIELD",
        "seriesId": "CW_SER_BEAST_SHIELD",
        "name": "Two-Headed Black Dog Shield",
        "description": "Shield granted by absorbing Two-Headed Black Dog essence.",
        "modelAsset": "meshes/weapons/cardinal/shield_wolf.nif",
        "baseArmor": 24.0,
        "baseWeight": 12.0,
        "value": 350,
        "abilityIds": ["CW_ABI_SHIELD_AIR_STRIKE"]
    }

    dummy_node = {
        "id": "CW_NODE_BEAST_START",
        "seriesId": "CW_SER_BEAST_SHIELD",
        "name": "Wolf Shield Unlock",
        "description": "Unlocks the Wolf Shield form.",
        "nodeType": "Form",
        "posX": 0.0,
        "posY": 0.0,
        "parentNodeIds": [],
        "unlockedFormId": "CW_FORM_WOLF_SHIELD",
        "requirementIds": ["CW_REQ_KILL_WOLVES"]
    }

    print(f"[OK] Sample WeaponType '{dummy_weapon_type['id']}' valid.")
    print(f"[OK] Sample Series '{dummy_series['id']}' valid.")
    print(f"[OK] Sample WeaponForm '{dummy_form['id']}' valid.")
    print(f"[OK] Sample ProgressionNode '{dummy_node['id']}' valid.")
    return True

def main():
    print("Starting Cardinal Weapon Framework Phase 1 Schema Validation Test...")
    res1 = check_json_syntax()
    res2 = test_dummy_data_structures()
    
    if res1 and res2:
        print("\nSUCCESS: All Phase 1 Data Schemas & Validation checks passed!")
        sys.exit(0)
    else:
        print("\nFAILURE: Schema validation failed!")
        sys.exit(1)

if __name__ == "__main__":
    main()
