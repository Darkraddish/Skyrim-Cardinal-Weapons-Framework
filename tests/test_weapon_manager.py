#!/usr/bin/env python3
"""
Unit Test Suite for Cardinal Weapon Framework Phase 3: Legendary Weapon Manager & Registration.
Tests dynamic registration, selection confirmation flow, active form swaps, and multi-weapon equip prevention.
"""

import sys

class DummyRegistry:
    def __init__(self):
        self.weapon_types = {}
        self.weapon_forms = {}

    def register_weapon_type(self, wt_id, name):
        self.weapon_types[wt_id] = {"id": wt_id, "name": name}

    def register_weapon_form(self, form_id, series_id, name):
        self.weapon_forms[form_id] = {"id": form_id, "seriesId": series_id, "name": name}

class DummySaveManager:
    def __init__(self):
        self.is_locked = False
        self.chosen_weapon_type = ""
        self.current_form_id = ""

    def lock_in_choice(self, wt_id):
        if self.is_locked:
            return False
        self.chosen_weapon_type = wt_id
        self.is_locked = True
        return True

class LegendaryWeaponManager:
    def __init__(self, registry, save_mgr):
        self.registry = registry
        self.save_mgr = save_mgr
        self.pending_choice = ""
        self.equipped_cardinal_item = ""

    def initiate_selection(self, wt_id):
        if self.save_mgr.is_locked:
            return False
        if wt_id not in self.registry.weapon_types:
            return False
        self.pending_choice = wt_id
        return True

    def confirm_choice(self, wt_id):
        if not self.pending_choice or self.pending_choice != wt_id:
            return False
        ok = self.save_mgr.lock_in_choice(wt_id)
        if ok:
            self.pending_choice = ""
            return True
        return False

    def swap_active_form(self, new_form_id):
        if new_form_id not in self.registry.weapon_forms:
            return False
        self.save_mgr.current_form_id = new_form_id
        return True

    def on_weapon_equipped(self, item_form_id):
        if item_form_id in self.registry.weapon_forms:
            # Multi-weapon prevention rule: If already equipped with another cardinal weapon, REJECT!
            if self.equipped_cardinal_item and self.equipped_cardinal_item != item_form_id:
                return False # Equipping second cardinal weapon blocked!
            self.equipped_cardinal_item = item_form_id
            return True
        return True # Non-cardinal item allowed

    def on_weapon_unequipped(self, item_form_id):
        if self.equipped_cardinal_item == item_form_id:
            self.equipped_cardinal_item = ""

def test_selection_flow():
    print("Test 1: Weapon Selection Confirmation Flow...")
    reg = DummyRegistry()
    save_mgr = DummySaveManager()
    lwm = LegendaryWeaponManager(reg, save_mgr)

    reg.register_weapon_type("CW_WT_SHIELD", "Cardinal Shield")

    # Initiate selection
    assert lwm.initiate_selection("CW_WT_SHIELD") == True
    assert lwm.pending_choice == "CW_WT_SHIELD"

    # Confirm selection
    assert lwm.confirm_choice("CW_WT_SHIELD") == True
    assert save_mgr.is_locked == True
    assert save_mgr.chosen_weapon_type == "CW_WT_SHIELD"
    assert lwm.pending_choice == ""

    # Attempting second selection -> MUST FAIL!
    assert lwm.initiate_selection("CW_WT_SWORD") == False
    print("  [PASS] Selection flow and single-choice confirmation verified.")

def test_form_swap():
    print("Test 2: Form Swapping...")
    reg = DummyRegistry()
    save_mgr = DummySaveManager()
    lwm = LegendaryWeaponManager(reg, save_mgr)

    reg.register_weapon_form("CW_FORM_WOLF_SHIELD", "CW_SER_BEAST_SHIELD", "Wolf Shield")
    reg.register_weapon_form("CW_FORM_FENRIR_SHIELD", "CW_SER_BEAST_SHIELD", "Fenrir Shield")

    assert lwm.swap_active_form("CW_FORM_WOLF_SHIELD") == True
    assert save_mgr.current_form_id == "CW_FORM_WOLF_SHIELD"

    assert lwm.swap_active_form("CW_FORM_FENRIR_SHIELD") == True
    assert save_mgr.current_form_id == "CW_FORM_FENRIR_SHIELD"
    print("  [PASS] Active form swap verified.")

def test_multi_weapon_equip_prevention():
    print("Test 3: Multi-Weapon Equip Prevention...")
    reg = DummyRegistry()
    save_mgr = DummySaveManager()
    lwm = LegendaryWeaponManager(reg, save_mgr)

    reg.register_weapon_form("CW_FORM_CARDINAL_SWORD_1", "CW_SER_SWORD", "Legendary Sword Form 1")
    reg.register_weapon_form("CW_FORM_CARDINAL_SWORD_2", "CW_SER_SWORD", "Legendary Sword Form 2")

    # Equip first cardinal weapon -> Allowed
    assert lwm.on_weapon_equipped("CW_FORM_CARDINAL_SWORD_1") == True
    assert lwm.equipped_cardinal_item == "CW_FORM_CARDINAL_SWORD_1"

    # Attempt to equip second cardinal weapon simultaneously -> MUST BE BLOCKED!
    assert lwm.on_weapon_equipped("CW_FORM_CARDINAL_SWORD_2") == False
    assert lwm.equipped_cardinal_item == "CW_FORM_CARDINAL_SWORD_1" # Still only first equipped!

    # Unequip first weapon
    lwm.on_weapon_unequipped("CW_FORM_CARDINAL_SWORD_1")
    assert lwm.equipped_cardinal_item == ""

    # Now equipping second weapon is allowed
    assert lwm.on_weapon_equipped("CW_FORM_CARDINAL_SWORD_2") == True
    print("  [PASS] Multi-weapon equip prevention enforced successfully.")

def main():
    print("=== Running Phase 3 Legendary Weapon Manager Unit Tests ===")
    test_selection_flow()
    test_form_swap()
    test_multi_weapon_equip_prevention()
    print("\nSUCCESS: All Phase 3 Legendary Weapon Manager Unit Tests Passed!")

if __name__ == "__main__":
    main()
