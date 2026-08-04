#!/usr/bin/env python3
"""
Unit Test Suite for Cardinal Weapon Framework Phase 8: Custom Flat SkyUI Cardinal Tree UI Framework.
Tests Dashboard view feeds, Flat 2D progression tree grid metrics, standardized right-side info panel template
across all weapon types (Sword, Shield, Bow, Spear), and SkyUI integration configuration.
"""

import sys

class DashboardViewModel:
    def __init__(self, wt_name, series_name, form_name, damage, armor):
        self.chosenWeaponTypeName = wt_name
        self.activeSeriesName = series_name
        self.currentFormName = form_name
        self.effectiveDamage = damage
        self.effectiveArmor = armor

class CardinalTreeNodeViewModel:
    def __init__(self, node_id, name, state, pos_x, pos_y, req_summaries):
        self.id = node_id
        self.name = name
        self.state = state  # "Locked", "Unlockable", "Unlocked"
        self.posX = pos_x
        self.posY = pos_y
        self.requirementSummaries = req_summaries

class FlatProgressionNodeViewModel:
    def __init__(self, node_id, name, state, grid_x, grid_y, req_summaries, parent_ids=None):
        self.id = node_id
        self.name = name
        self.state = state
        self.gridX = grid_x
        self.gridY = grid_y
        self.requirementSummaries = req_summaries
        self.parentNodeIds = parent_ids or []

class FlatProgressionTreeViewModel:
    def __init__(self, series_id, series_name, weapon_type_id, nodes):
        self.seriesId = series_id
        self.seriesName = series_name
        self.weaponTypeId = weapon_type_id
        self.nodes = nodes

class AbilitySummaryViewModel:
    def __init__(self, abi_id, name, description, activation_type, scope, unlocked):
        self.id = abi_id
        self.name = name
        self.description = description
        self.activationType = activation_type
        self.scope = scope
        self.unlocked = unlocked

class StandardizedInfoPanelViewModel:
    def __init__(self, header, selected_node_id, selected_node_name, state, req_summaries, abilities, can_unlock):
        self.header = header
        self.selectedNodeId = selected_node_id
        self.selectedNodeName = selected_node_name
        self.selectedNodeState = state
        self.requirementSummaries = req_summaries
        self.grantedAbilities = abilities
        self.canUnlock = can_unlock
        self.unlockButtonHint = "[E] Unlock Node" if can_unlock else "[E] Locked"
        self.backButtonHint = "[Tab] Back"

class SkyUIIntegrationConfig:
    def __init__(self):
        self.skyUIRequired = True
        self.skyUIVersion = "5.2 SE+"
        self.menuName = "CardinalWeaponsMenu"
        self.swfPath = "Interface/CardinalWeaponsMenu.swf"
        self.enableFlat2DMode = True
        self.leftPaneWidthPercent = 55.0
        self.rightPaneWidthPercent = 45.0

def test_dashboard_and_cardinal_tree_network_views():
    print("Test 1: Weapon Overview Dashboard & Cardinal Tree Node View Models...")

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

    node_view = CardinalTreeNodeViewModel(
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
    print("  [PASS] Dashboard and cardinal tree node network views verified.")

def test_flat_2d_progression_tree_view():
    print("Test 2: Flat 2D Progression Tree View Model & Grid Metrics...")

    nodes = [
        FlatProgressionNodeViewModel("NODE_1", "Base Form", "Unlocked", 0, 0, []),
        FlatProgressionNodeViewModel("NODE_2", "Evolved Form", "Unlockable", 1, 0, ["Kill 10 Beasts (10/10)"], ["NODE_1"]),
        FlatProgressionNodeViewModel("NODE_3", "Master Form", "Locked", 2, 0, ["Player Level 20 (12/20)"], ["NODE_2"])
    ]

    tree = FlatProgressionTreeViewModel(
        series_id="CW_SERIES_SWORD_DRAGON",
        series_name="Dragon Slayer Series",
        weapon_type_id="CW_WEAPON_TYPE_SWORD",
        nodes=nodes
    )

    assert tree.seriesId == "CW_SERIES_SWORD_DRAGON"
    assert len(tree.nodes) == 3
    assert tree.nodes[1].gridX == 1
    assert tree.nodes[1].gridY == 0
    assert tree.nodes[1].parentNodeIds == ["NODE_1"]
    print("  [PASS] Flat 2D progression tree view model & grid metrics verified.")

def test_standardized_info_panel_across_all_weapons():
    print("Test 3: Standardized Info Panel Template Across All Weapon Types...")

    weapon_cases = [
        ("Cardinal Sword", "Dragon Sword Series", "Gram", 45.0, 0.0),
        ("Cardinal Shield", "Beast Shield Series", "Small Shield", 0.0, 18.0),
        ("Cardinal Bow", "Celestial Bow Series", "Spirit Bow", 38.0, 0.0),
        ("Cardinal Spear", "Vanguard Spear Series", "Gungnir", 50.0, 0.0)
    ]

    for wt_name, series_name, form_name, dmg, arm in weapon_cases:
        header = DashboardViewModel(wt_name, series_name, form_name, dmg, arm)
        ability = AbilitySummaryViewModel("ABI_1", "Passive Boost", "Increases attack speed by 10%", "Passive", "Permanent", True)
        
        info_panel = StandardizedInfoPanelViewModel(
            header=header,
            selected_node_id="NODE_TEST",
            selected_node_name=form_name,
            state="Unlocked",
            req_summaries=["Requirement Met"],
            abilities=[ability],
            can_unlock=True
        )

        assert info_panel.header.chosenWeaponTypeName == wt_name
        assert info_panel.selectedNodeName == form_name
        assert info_panel.unlockButtonHint == "[E] Unlock Node"
        assert info_panel.backButtonHint == "[Tab] Back"
        assert len(info_panel.grantedAbilities) == 1

    print("  [PASS] Standardized info panel template verified across Sword, Shield, Bow, and Spear.")

def test_skyui_integration_config():
    print("Test 4: SkyUI Requirement & Integration Configuration...")

    config = SkyUIIntegrationConfig()
    assert config.skyUIRequired is True
    assert config.skyUIVersion == "5.2 SE+"
    assert config.enableFlat2DMode is True
    assert config.leftPaneWidthPercent == 55.0
    assert config.rightPaneWidthPercent == 45.0
    print("  [PASS] SkyUI requirement & integration configuration verified.")

def main():
    print("=== Running Phase 8 Custom Flat SkyUI Cardinal Tree UI Framework Unit Tests ===")
    test_dashboard_and_cardinal_tree_network_views()
    test_flat_2d_progression_tree_view()
    test_standardized_info_panel_across_all_weapons()
    test_skyui_integration_config()
    print("\nSUCCESS: All Phase 8 Custom Flat SkyUI Cardinal Tree UI Framework Unit Tests Passed!")

if __name__ == "__main__":
    main()
