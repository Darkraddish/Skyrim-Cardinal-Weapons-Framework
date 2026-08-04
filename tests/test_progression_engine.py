#!/usr/bin/env python3
"""
Unit Test Suite for Cardinal Weapon Framework Phase 5: Progression & Sub-System Engines.
Tests Evolution, Level, Refinement, and Mastery independent growth systems.
"""

import sys
import math

class SubSystemsState:
    def __init__(self):
        self.levelBonus = 0.0
        self.refinementLevel = 0
        self.masteryXp = 0.0
        self.masteryMultiplier = 1.0

class ProgressionEngine:
    def __init__(self):
        self.sub_systems = SubSystemsState()
        self.unlocked_nodes = set()
        self.active_form = ""
        self.convertible_resolutions = {}

    def add_flat_level_bonus(self, bonus):
        self.sub_systems.levelBonus += bonus

    def upgrade_refinement(self):
        self.sub_systems.refinementLevel += 1

    def get_refinement_multiplier(self):
        return 1.0 + (self.sub_systems.refinementLevel * 0.15)

    def add_mastery_xp(self, xp):
        self.sub_systems.masteryXp += xp
        self.sub_systems.masteryMultiplier = 1.0 + math.log10(1.0 + self.sub_systems.masteryXp / 100.0) * 0.5

    def calculate_effective_stat(self, base_val):
        level_b = self.sub_systems.levelBonus
        ref_m = self.get_refinement_multiplier()
        mas_m = self.sub_systems.masteryMultiplier
        return (base_val + level_b) * ref_m * mas_m

    def unlock_node(self, node_id, unlocked_form_id=None):
        self.unlocked_nodes.add(node_id)
        if unlocked_form_id:
            self.active_form = unlocked_form_id

    def resolve_convertible_node(self, node_id, outcome_form_id):
        self.convertible_resolutions[node_id] = outcome_form_id
        self.active_form = outcome_form_id

def test_independent_subsystems():
    print("Test 1: Independent Sub-System Growth & Combined Stat Formula...")
    pe = ProgressionEngine()

    base_damage = 20.0
    # Initial stat with 0 bonuses
    eff1 = pe.calculate_effective_stat(base_damage)
    assert eff1 == 20.0

    # 1. Level System: Flat +10 damage
    pe.add_flat_level_bonus(10.0)
    assert pe.sub_systems.levelBonus == 10.0
    eff2 = pe.calculate_effective_stat(base_damage)
    assert eff2 == (20.0 + 10.0) * 1.0 * 1.0 # 30.0

    # 2. Refinement System: Tier 2 (+30% multiplier -> 1.30)
    pe.upgrade_refinement()
    pe.upgrade_refinement()
    assert pe.sub_systems.refinementLevel == 2
    assert abs(pe.get_refinement_multiplier() - 1.30) < 0.001
    eff3 = pe.calculate_effective_stat(base_damage)
    assert abs(eff3 - (30.0 * 1.30)) < 0.001 # 39.0

    # 3. Mastery System: Add 900 XP (log10(1 + 9) = 1.0 -> 1.0 + 0.5 = 1.5 multiplier)
    pe.add_mastery_xp(900.0)
    assert abs(pe.sub_systems.masteryMultiplier - 1.5) < 0.001

    # Formula: (20 + 10) * 1.30 * 1.5 = 30 * 1.95 = 58.5
    eff4 = pe.calculate_effective_stat(base_damage)
    assert abs(eff4 - 58.5) < 0.01
    print("  [PASS] Independent growth sub-systems formula verified.")

def test_evolution_and_convertible_nodes():
    print("Test 2: Evolution & Convertible Node Resolution...")
    pe = ProgressionEngine()

    pe.unlock_node("NODE_WOLF_SHIELD", unlocked_form_id="FORM_WOLF_SHIELD")
    assert pe.active_form == "FORM_WOLF_SHIELD"

    # Convertible node resolution in-place
    pe.resolve_convertible_node("NODE_CONVERTIBLE_1", outcome_form_id="FORM_RESOLVED_SHIELD")
    assert pe.active_form == "FORM_RESOLVED_SHIELD"
    assert pe.convertible_resolutions["NODE_CONVERTIBLE_1"] == "FORM_RESOLVED_SHIELD"
    print("  [PASS] Evolution and convertible node resolution verified.")

def main():
    print("=== Running Phase 5 Progression Engine Unit Tests ===")
    test_independent_subsystems()
    test_evolution_and_convertible_nodes()
    print("\nSUCCESS: All Phase 5 Progression Engine Unit Tests Passed!")

if __name__ == "__main__":
    main()
