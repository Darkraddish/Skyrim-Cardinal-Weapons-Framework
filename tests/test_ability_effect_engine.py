#!/usr/bin/env python3
"""
Unit Test Suite for Cardinal Weapon Framework Phase 6: Ability & Effect Engine.
Tests form-bound vs permanent ability lifecycle and simultaneous positive/negative stat effects.
"""

import sys

class AbilityData:
    def __init__(self, abi_id, name, scope):
        self.id = abi_id
        self.name = name
        self.scope = scope # "Equip", "Active", "Passive", "Master"

    def is_form_bound(self):
        return self.scope in ["Equip", "Active"]

    def is_permanent(self):
        return self.scope in ["Passive", "Master"]

class EffectData:
    def __init__(self, eff_id, target_av, val):
        self.id = eff_id
        self.targetActorValue = target_av
        self.value = val # positive boost or negative drawback

class AbilityEffectEngine:
    def __init__(self):
        self.form_bound_active = set()
        self.permanent_active = set()
        self.applied_effects = {}

    def grant_ability(self, abi):
        if abi.is_form_bound():
            self.form_bound_active.add(abi.id)
        elif abi.is_permanent():
            self.permanent_active.add(abi.id)

    def on_form_swapped(self, old_form_abilities, new_form_abilities):
        # Revoke old form-bound abilities
        for a_id in old_form_abilities:
            self.form_bound_active.discard(a_id)

        # Grant new form-bound abilities
        for a_id in new_form_abilities:
            self.form_bound_active.add(a_id)

    def apply_effect(self, eff):
        self.applied_effects[eff.id] = eff

    def calculate_net_actor_values(self):
        net = {}
        for eff in self.applied_effects.values():
            net[eff.targetActorValue] = net.get(eff.targetActorValue, 0.0) + eff.value
        return net

def test_form_bound_vs_permanent_abilities():
    print("Test 1: Form-Bound vs Permanent Ability Lifecycle...")
    engine = AbilityEffectEngine()

    abi_equip = AbilityData("ABI_AIR_STRIKE", "Air Strike", "Equip")
    abi_master = AbilityData("ABI_MASTER_SHIELD", "Master Shield Passive", "Master")

    # Grant permanent master skill
    engine.grant_ability(abi_master)
    assert "ABI_MASTER_SHIELD" in engine.permanent_active

    # Form 1 equipped (grants AIR_STRIKE)
    engine.on_form_swapped([], ["ABI_AIR_STRIKE"])
    assert "ABI_AIR_STRIKE" in engine.form_bound_active
    assert "ABI_MASTER_SHIELD" in engine.permanent_active

    # Swap to Form 2 (grants ABI_FIRE_AURA, revokes AIR_STRIKE)
    engine.on_form_swapped(["ABI_AIR_STRIKE"], ["ABI_FIRE_AURA"])
    assert "ABI_AIR_STRIKE" not in engine.form_bound_active # Form-bound revoked!
    assert "ABI_FIRE_AURA" in engine.form_bound_active      # New form-bound active!
    assert "ABI_MASTER_SHIELD" in engine.permanent_active    # Permanent remains active!
    print("  [PASS] Form-bound vs permanent ability scoping verified.")

def test_simultaneous_boosts_and_drawbacks():
    print("Test 2: Simultaneous Positive Boosts & Negative Drawbacks...")
    engine = AbilityEffectEngine()

    eff_boost = EffectData("EFF_ARMOR_BOOST", "DamageResist", 25.0)   # +25 Armor
    eff_curse = EffectData("EFF_MAGICKA_CURSE", "MagickaRate", -15.0)  # -15 Magicka Regen

    engine.apply_effect(eff_boost)
    engine.apply_effect(eff_curse)

    net = engine.calculate_net_actor_values()
    assert net["DamageResist"] == 25.0
    assert net["MagickaRate"] == -15.0
    print("  [PASS] Positive boosts and negative drawbacks aggregated successfully.")

def main():
    print("=== Running Phase 6 Ability & Effect Engine Unit Tests ===")
    test_form_bound_vs_permanent_abilities()
    test_simultaneous_boosts_and_drawbacks()
    print("\nSUCCESS: All Phase 6 Ability & Effect Engine Unit Tests Passed!")

if __name__ == "__main__":
    main()
