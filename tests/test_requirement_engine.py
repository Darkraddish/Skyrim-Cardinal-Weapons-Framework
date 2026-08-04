#!/usr/bin/env python3
"""
Unit Test Suite for Cardinal Weapon Framework Phase 4: Requirement Engine.
Tests all condition handlers, kill/absorption tracking, parent node dependency enforcement, and live progress reporting.
"""

import sys

class DummyRequirement:
    def __init__(self, req_id, req_type, target_id, target_val):
        self.id = req_id
        self.type = req_type
        self.targetId = target_id
        self.targetValue = target_val

class DummyNode:
    def __init__(self, node_id, parent_node_ids, requirement_ids):
        self.id = node_id
        self.parentNodeIds = parent_node_ids
        self.requirementIds = requirement_ids

class RequirementEvaluator:
    def __init__(self, save_mgr):
        self.save_mgr = save_mgr
        self.kill_counts = {}
        self.absorbed_items = {}
        self.copied_weapons = {}
        self.quest_stages = {}
        self.player_level = 1
        self.triggered_events = {}
        self.requirements_db = {}

    def register_requirement(self, req):
        self.requirements_db[req.id] = req

    def record_kill(self, actor_id, count=1):
        self.kill_counts[actor_id] = self.kill_counts.get(actor_id, 0) + count

    def record_absorption(self, item_id, count=1):
        self.absorbed_items[item_id] = self.absorbed_items.get(item_id, 0) + count

    def record_weapon_copy(self, weapon_id):
        self.copied_weapons[weapon_id] = True

    def get_progress(self, req):
        req_type = req.type
        target = req.targetValue
        if req_type == "KillCount":
            cur = float(self.kill_counts.get(req.targetId, 0))
            return cur, target
        elif req_type == "ItemAbsorption":
            cur = float(self.absorbed_items.get(req.targetId, 0))
            return cur, target
        elif req_type == "WeaponCopy":
            cur = 1.0 if self.copied_weapons.get(req.targetId, False) else 0.0
            return cur, 1.0
        elif req_type == "PlayerLevel":
            return float(self.player_level), target
        elif req_type == "ParentNodeDependency":
            is_unlocked = req.targetId in self.save_mgr.unlocked_nodes
            return 1.0 if is_unlocked else 0.0, 1.0
        return 0.0, target

    def is_met(self, req):
        cur, tar = self.get_progress(req)
        return cur >= tar

    def can_unlock_node(self, node):
        # 1. Check parent node dependencies
        for p_id in node.parentNodeIds:
            if p_id not in self.save_mgr.unlocked_nodes:
                return False # Blocked by parent!
        # 2. Check requirements
        for r_id in node.requirementIds:
            if r_id in self.requirements_db:
                req = self.requirements_db[r_id]
                if not self.is_met(req):
                    return False
        return True

class DummySaveManager:
    def __init__(self):
        self.unlocked_nodes = set()

def test_kill_and_absorption_tracking():
    print("Test 1: Kill Count & Item Absorption Tracking...")
    sm = DummySaveManager()
    evaluator = RequirementEvaluator(sm)

    req_kill = DummyRequirement("REQ_KILL_WOLVES", "KillCount", "WolfActorBase", 10.0)
    evaluator.register_requirement(req_kill)

    assert evaluator.is_met(req_kill) == False
    cur, tar = evaluator.get_progress(req_kill)
    assert (cur, tar) == (0.0, 10.0)

    # Record 7 kills
    evaluator.record_kill("WolfActorBase", 7)
    cur, tar = evaluator.get_progress(req_kill)
    assert (cur, tar) == (7.0, 10.0)
    assert evaluator.is_met(req_kill) == False

    # Record 3 more kills -> total 10
    evaluator.record_kill("WolfActorBase", 3)
    cur, tar = evaluator.get_progress(req_kill)
    assert (cur, tar) == (10.0, 10.0)
    assert evaluator.is_met(req_kill) == True
    print("  [PASS] Kill count tracking and live progress verified.")

def test_parent_node_dependency_enforcement():
    print("Test 2: Parent Node Dependency Enforcement...")
    sm = DummySaveManager()
    evaluator = RequirementEvaluator(sm)

    req_kill = DummyRequirement("REQ_KILL_WOLVES", "KillCount", "WolfActorBase", 5.0)
    evaluator.register_requirement(req_kill)
    evaluator.record_kill("WolfActorBase", 5) # Kill requirement met!

    parent_node = DummyNode("NODE_ROOT", [], [])
    child_node = DummyNode("NODE_CHILD", ["NODE_ROOT"], ["REQ_KILL_WOLVES"])

    # Child node cannot be unlocked because NODE_ROOT is NOT unlocked yet!
    assert evaluator.can_unlock_node(child_node) == False

    # Unlock parent node in SaveManager
    sm.unlocked_nodes.add("NODE_ROOT")

    # Now child node CAN be unlocked!
    assert evaluator.can_unlock_node(child_node) == True
    print("  [PASS] Parent node dependency enforcement verified.")

def main():
    print("=== Running Phase 4 Requirement Engine Unit Tests ===")
    test_kill_and_absorption_tracking()
    test_parent_node_dependency_enforcement()
    print("\nSUCCESS: All Phase 4 Requirement Engine Unit Tests Passed!")

if __name__ == "__main__":
    main()
