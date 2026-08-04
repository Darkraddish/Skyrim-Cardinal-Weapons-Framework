#!/usr/bin/env python3
"""
C++ Architecture and Decoupling Validation Test for Cardinal Weapon Framework Phase 1.
"""

import os
import re
import sys

BASE_DIR = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
INCLUDE_DIR = os.path.join(BASE_DIR, "include", "CWFramework")
SRC_DIR = os.path.join(BASE_DIR, "src")
PAPYRUS_DIR = os.path.join(BASE_DIR, "scripts", "source")

FORBIDDEN_TERMS = ["Shield", "Sword", "Bow", "Spear", "Dwemer", "Dragon", "Beast", "Naofumi", "Filolial"]

def check_file_decoupling(filepath):
    rel_path = os.path.relpath(filepath, BASE_DIR)
    with open(filepath, "r", encoding="utf-8") as f:
        content = f.read()
    
    # Check that core C++ files do not contain hardcoded weapon content references
    found_forbidden = []
    for term in FORBIDDEN_TERMS:
        # Avoid matching enum names or standard words if any, but ensure no hardcoded weapon logic exists
        pattern = r'\b' + term + r'\b'
        matches = re.findall(pattern, content)
        if matches:
            found_forbidden.append(term)
            
    if found_forbidden:
        print(f"[FAIL] {rel_path} contains hardcoded content references: {found_forbidden}")
        return False
    else:
        print(f"[OK] {rel_path} decoupled (0 hardcoded weapon references).")
        return True

def main():
    print("=== Running C++ Decoupling & Architecture Audit ===")
    all_ok = True
    
    cpp_files = [
        os.path.join(INCLUDE_DIR, "DataModels.h"),
        os.path.join(INCLUDE_DIR, "Interfaces.h"),
        os.path.join(INCLUDE_DIR, "PublicAPI.h"),
        os.path.join(INCLUDE_DIR, "Registry.h"),
        os.path.join(SRC_DIR, "Registry.cpp"),
        os.path.join(INCLUDE_DIR, "SchemaValidator.h"),
        os.path.join(SRC_DIR, "SchemaValidator.cpp"),
        os.path.join(INCLUDE_DIR, "SaveData.h"),
        os.path.join(INCLUDE_DIR, "SaveManager.h"),
        os.path.join(SRC_DIR, "SaveManager.cpp"),
        os.path.join(INCLUDE_DIR, "LegendaryWeaponManager.h"),
        os.path.join(SRC_DIR, "LegendaryWeaponManager.cpp"),
        os.path.join(PAPYRUS_DIR, "CWFramework_API.psc"),
        os.path.join(PAPYRUS_DIR, "CWFramework_DataTypes.psc"),
        os.path.join(PAPYRUS_DIR, "CWFramework_SaveAPI.psc"),
        os.path.join(PAPYRUS_DIR, "CWFramework_WeaponManagerAPI.psc"),
    ]
    
    for fpath in cpp_files:
        if not os.path.exists(fpath):
            print(f"[FAIL] Missing architecture file: {fpath}")
            all_ok = False
            continue
        if not check_file_decoupling(fpath):
            all_ok = False
            
    if all_ok:
        print("\nSUCCESS: All C++ & Papyrus files are 100% decoupled and weapon-agnostic!")
        sys.exit(0)
    else:
        print("\nFAILURE: Decoupling check failed!")
        sys.exit(1)

if __name__ == "__main__":
    main()
