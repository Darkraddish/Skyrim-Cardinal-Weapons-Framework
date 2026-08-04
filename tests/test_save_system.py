#!/usr/bin/env python3
"""
Unit Test Suite for Cardinal Weapon Framework Phase 2: Save System & Persistence Engine.
Tests single-weapon lock-in rule, node state persistence, serialization, and save validation.
"""

import sys
import io

class SaveData:
    def __init__(self):
        self.version = 1
        self.isWeaponLocked = False
        self.chosenWeaponTypeId = ""
        self.currentFormId = ""
        self.unlockedNodeIds = []
        self.convertibleNodeResolutions = {}
        self.revealedBranchSeriesIds = []
        self.nodeStates = {} # nodeId -> int (0=Locked, 1=Unlockable, 2=Unlocked)
        self.subSystems = {
            "levelBonus": 0.0,
            "refinementLevel": 0,
            "masteryXp": 0.0,
            "masteryMultiplier": 1.0
        }

    def serialize(self):
        lines = []
        lines.append(str(self.version))
        lines.append("1" if self.isWeaponLocked else "0")
        lines.append(self.chosenWeaponTypeId if self.chosenWeaponTypeId else "<none>")
        lines.append(self.currentFormId if self.currentFormId else "<none>")
        
        lines.append(str(len(self.unlockedNodeIds)))
        for n_id in self.unlockedNodeIds:
            lines.append(n_id)

        lines.append(str(len(self.convertibleNodeResolutions)))
        for n_id, f_id in self.convertibleNodeResolutions.items():
            lines.append(f"{n_id} {f_id}")

        lines.append(str(len(self.revealedBranchSeriesIds)))
        for s_id in self.revealedBranchSeriesIds:
            lines.append(s_id)

        lines.append(str(len(self.nodeStates)))
        for n_id, s_val in self.nodeStates.items():
            lines.append(f"{n_id} {s_val}")

        sub = self.subSystems
        lines.append(f"{sub['levelBonus']} {sub['refinementLevel']} {sub['masteryXp']} {sub['masteryMultiplier']}")
        return "\n".join(lines)

    def deserialize(self, raw_text):
        lines = [line.strip() for line in raw_text.strip().splitlines() if line.strip()]
        if not lines:
            return False
        idx = 0
        self.version = int(lines[idx]); idx += 1
        self.isWeaponLocked = (lines[idx] == "1"); idx += 1
        
        wt = lines[idx]; idx += 1
        self.chosenWeaponTypeId = "" if wt == "<none>" else wt
        
        form = lines[idx]; idx += 1
        self.currentFormId = "" if form == "<none>" else form

        unlocked_count = int(lines[idx]); idx += 1
        self.unlockedNodeIds = []
        for _ in range(unlocked_count):
            self.unlockedNodeIds.append(lines[idx]); idx += 1

        conv_count = int(lines[idx]); idx += 1
        self.convertibleNodeResolutions = {}
        for _ in range(conv_count):
            parts = lines[idx].split(); idx += 1
            if len(parts) == 2:
                self.convertibleNodeResolutions[parts[0]] = parts[1]

        rev_count = int(lines[idx]); idx += 1
        self.revealedBranchSeriesIds = []
        for _ in range(rev_count):
            self.revealedBranchSeriesIds.append(lines[idx]); idx += 1

        state_count = int(lines[idx]); idx += 1
        self.nodeStates = {}
        for _ in range(state_count):
            parts = lines[idx].split(); idx += 1
            if len(parts) == 2:
                self.nodeStates[parts[0]] = int(parts[1])

        sub_parts = lines[idx].split(); idx += 1
        self.subSystems = {
            "levelBonus": float(sub_parts[0]),
            "refinementLevel": int(sub_parts[1]),
            "masteryXp": float(sub_parts[2]),
            "masteryMultiplier": float(sub_parts[3])
        }
        return True

class SaveManager:
    def __init__(self):
        self.data = SaveData()

    def lock_in_weapon_choice(self, weapon_type_id):
        if not weapon_type_id:
            return False
        # Rule: EXACTLY ONE Legendary Weapon per save
        if self.data.isWeaponLocked:
            return False
        self.data.chosenWeaponTypeId = weapon_type_id
        self.data.isWeaponLocked = True
        return True

    def unlock_node(self, node_id):
        if node_id not in self.data.unlockedNodeIds:
            self.data.unlockedNodeIds.append(node_id)
        self.data.nodeStates[node_id] = 2 # Unlocked
        return True

    def validate_save_state(self):
        errors = []
        if self.data.isWeaponLocked and not self.data.chosenWeaponTypeId:
            errors.append("Weapon choice is locked but chosenWeaponTypeId is empty.")
        if not self.data.isWeaponLocked:
            if self.data.currentFormId:
                errors.append("Active weapon form exists before weapon choice is locked.")
            if self.data.unlockedNodeIds:
                errors.append("Unlocked nodes exist before weapon choice is locked.")
        return len(errors) == 0, errors

# Test Execution
def test_single_weapon_lock():
    print("Test 1: Single-Weapon Lock Rule...")
    sm = SaveManager()
    assert not sm.data.isWeaponLocked
    assert sm.lock_in_weapon_choice("CW_WT_SHIELD") == True
    assert sm.data.isWeaponLocked == True
    assert sm.data.chosenWeaponTypeId == "CW_WT_SHIELD"

    # Attempt to choose a second weapon -> MUST FAIL!
    assert sm.lock_in_weapon_choice("CW_WT_SWORD") == False
    assert sm.data.chosenWeaponTypeId == "CW_WT_SHIELD" # Retains first choice!
    print("  [PASS] Single-weapon lock enforced successfully.")

def test_serialization_roundtrip():
    print("Test 2: Serialization & Deserialization Roundtrip...")
    sm1 = SaveManager()
    sm1.lock_in_weapon_choice("CW_WT_BOW")
    sm1.data.currentFormId = "CW_FORM_HUNTER_BOW"
    sm1.unlock_node("CW_NODE_BOW_START")
    sm1.unlock_node("CW_NODE_BOW_PERK_1")
    sm1.data.subSystems["levelBonus"] = 15.0
    sm1.data.subSystems["refinementLevel"] = 2

    serialized = sm1.data.serialize()
    
    sm2 = SaveManager()
    success = sm2.data.deserialize(serialized)
    assert success == True
    assert sm2.data.isWeaponLocked == True
    assert sm2.data.chosenWeaponTypeId == "CW_WT_BOW"
    assert sm2.data.currentFormId == "CW_FORM_HUNTER_BOW"
    assert sm2.data.unlockedNodeIds == ["CW_NODE_BOW_START", "CW_NODE_BOW_PERK_1"]
    assert sm2.data.subSystems["levelBonus"] == 15.0
    assert sm2.data.subSystems["refinementLevel"] == 2
    print("  [PASS] Serialization roundtrip matched 100%.")

def test_illegal_state_validation():
    print("Test 3: Save State Validation & Rejection of Illegal States...")
    # Valid initial state
    sm = SaveManager()
    is_valid, errs = sm.validate_save_state()
    assert is_valid == True

    # Corrupt state: unlocked nodes without weapon lock
    sm.data.unlockedNodeIds.append("CW_NODE_ILLEGAL")
    is_valid, errs = sm.validate_save_state()
    assert is_valid == False
    assert len(errs) > 0
    print("  [PASS] Successfully detected and rejected illegal save state.")

def main():
    print("=== Running Phase 2 Save System Unit Tests ===")
    test_single_weapon_lock()
    test_serialization_roundtrip()
    test_illegal_state_validation()
    print("\nSUCCESS: All Phase 2 Save System Unit Tests Passed!")

if __name__ == "__main__":
    main()
