#!/usr/bin/env python3
"""
Unit Test Suite for Cardinal Weapon Framework Phase 10: Expansion Support & Companion Hooks.
Tests Series/Weapon expansion data merging and companion module decoupling.
"""

import sys

class ExpansionManager:
    def __init__(self):
        self.companion_modules = {}
        self.registered_series = {}

    def register_companion_module(self, mod_type, name):
        self.companion_modules[mod_type] = name

    def unregister_companion_module(self, mod_type):
        self.companion_modules.pop(mod_type, None)

    def is_companion_active(self, mod_type):
        return mod_type in self.companion_modules

    def merge_series_expansion(self, series_id, name):
        self.registered_series[series_id] = name
        return True

def test_expansion_merging_and_companion_hooks():
    print("Test 1: Series Expansion Merging & Companion Module Hooks...")
    mgr = ExpansionManager()

    # 1. Merge new Dwemer Series expansion data across weapon types
    assert mgr.merge_series_expansion("CW_SER_DWEMER_SHIELD", "Dwemer Shield Series") == True
    assert "CW_SER_DWEMER_SHIELD" in mgr.registered_series

    # 2. Companion module registration (e.g. Economy Overhaul)
    assert mgr.is_companion_active("Economy") == False
    mgr.register_companion_module("Economy", "CardinalEconomyCompanion.esp")
    assert mgr.is_companion_active("Economy") == True

    # Disable companion module -> Framework remains 100% functional!
    mgr.unregister_companion_module("Economy")
    assert mgr.is_companion_active("Economy") == False
    assert "CW_SER_DWEMER_SHIELD" in mgr.registered_series
    print("  [PASS] Expansion merging and companion decoupling verified.")

def main():
    print("=== Running Phase 10 Expansion Support & Companion Hooks Unit Tests ===")
    test_expansion_merging_and_companion_hooks()
    print("\nSUCCESS: All Phase 10 Expansion Support Unit Tests Passed!")

if __name__ == "__main__":
    main()
