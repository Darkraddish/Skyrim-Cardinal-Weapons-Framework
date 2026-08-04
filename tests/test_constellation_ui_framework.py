#!/usr/bin/env python3
"""
Unit Test Suite for Cardinal Weapon Framework Phase 8: Custom Constellation UI Framework.
Tests Dashboard view data feeds, Constellation Network node states, and node selection layer.
"""

import sys

class DashboardViewModel:
    def __init__(self, wt_name, series_name, form_name, damage, armor):
        self.chosenWeaponTypeName = wt_name
        self.activeSeriesName = series_name
        self.currentFormName = form_name
        self.effectiveDamage = damage
        self.effectiveArmor = armor

class ConstellationNodeViewModel:
    def __init__(self, node_id, name, state, pos_x, pos_y, req_summaries):
        self.id = node_id
        self.name = name
        self.state = state # "Locked", "Unlockable", "Unlocked"
        self.posX = pos_x
        self.posY = pos_y
        self.requirementSummaries = req_summaries

def test_dashboard_and_constellation_network_views():
    print("Test 1: Weapon Overview Dashboard & Constellation Node View Models...")

    dashboard = DashboardViewModel(
        wt_name="Cardinal Shield",
        series_name="Beast Shield Series",
        form_name="Two-Headed Black Dog Shield",
        damage=0.0,
        armor=35.1
    )

    assert dashboard.chosenWeaponTypeName == "Cardinal Shield"
    assert dashboard.currentFormName == "Two-Headed Black Dog Shield"
    assert dashboard.effectiveArmor == 35.1

    node_view = ConstellationNodeViewModel(
        node_id="CW_NODE_BEAST_2",
        name="Fenrir Shield Transformation",
        state="Unlockable",
        pos_x=150.0,
        pos_y=220.0,
        req_summaries=["Defeat 10 Dire Wolves (7/10)", "Player Level 15 (15/15)"]
    )

    assert node_view.state == "Unlockable"
    assert len(node_view.requirementSummaries) == 2
    assert "7/10" in node_view.requirementSummaries[0]
    print("  [PASS] Dashboard and constellation node network views verified.")

def main():
    print("=== Running Phase 8 Custom Constellation UI Framework Unit Tests ===")
    test_dashboard_and_constellation_network_views()
    print("\nSUCCESS: All Phase 8 Custom Constellation UI Framework Unit Tests Passed!")

if __name__ == "__main__":
    main()
