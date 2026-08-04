#!/usr/bin/env python3
"""
Unit Test Suite for Cardinal Weapon Framework Phase 7: Content Validation Engine.
Tests Two-Stage Validation and Soft-Failure Graceful Degradation.
"""

import sys

class ContentValidationEngine:
    def __init__(self):
        self.registry_items = set()
        self.unavailable_items = {}

    def register_item(self, item_id):
        self.registry_items.add(item_id)

    def mark_unavailable(self, item_id, reason):
        self.unavailable_items[item_id] = reason

    def is_available(self, item_id):
        return item_id not in self.unavailable_items

    def stage1_load_validation(self):
        # Stage 1 check
        errors = []
        for item_id in self.registry_items:
            if not item_id:
                errors.append("Empty item ID in registry")
        return len(errors) == 0, errors

    def stage2_pre_unlock_verification(self, node_id):
        # Stage 2 runtime check prior to choice confirmation
        if node_id in self.unavailable_items:
            return False, f"Item '{node_id}' unavailable: {self.unavailable_items[node_id]}"
        if node_id not in self.registry_items:
            return False, f"Item '{node_id}' missing from registry"
        return True, "OK"

def test_two_stage_validation_and_soft_failure():
    print("Test 1: Stage 1 Load & Stage 2 Pre-Unlock Validation...")
    engine = ContentValidationEngine()

    engine.register_item("CW_NODE_WOLF_SHIELD")
    engine.register_item("CW_NODE_BROKEN_ITEM")

    # Stage 1 check -> OK
    ok, errs = engine.stage1_load_validation()
    assert ok == True

    # Pre-unlock check on valid node -> OK
    ok, msg = engine.stage2_pre_unlock_verification("CW_NODE_WOLF_SHIELD")
    assert ok == True

    # Mark BROKEN_ITEM as unavailable (e.g. missing texture / deleted mod pack mid-session)
    engine.mark_unavailable("CW_NODE_BROKEN_ITEM", "Texture mesh missing")

    # Pre-unlock check on broken node -> Gracefully fails without crash!
    ok, msg = engine.stage2_pre_unlock_verification("CW_NODE_BROKEN_ITEM")
    assert ok == False
    assert "Texture mesh missing" in msg
    print("  [PASS] Two-stage validation and soft-failure isolation verified.")

def main():
    print("=== Running Phase 7 Content Validation Engine Unit Tests ===")
    test_two_stage_validation_and_soft_failure()
    print("\nSUCCESS: All Phase 7 Content Validation Engine Unit Tests Passed!")

if __name__ == "__main__":
    main()
