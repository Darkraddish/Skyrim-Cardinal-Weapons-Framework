#pragma once

#include "DataModels.h"
#include "Registry.h"
#include "SaveManager.h"
#include "RequirementEvaluator.h"
#include "ProgressionEngine.h"
#include "AbilityEffectEngine.h"
#include <shared_mutex>
#include <string>
#include <vector>

namespace CWFramework
{
    struct DashboardViewModel
    {
        std::string chosenWeaponTypeName;
        std::string activeSeriesName;
        std::string currentFormName;
        std::string currentFormIconAsset;
        double levelBonus{ 0.0 };
        uint32_t refinementLevel{ 0 };
        double masteryMultiplier{ 1.0 };
        double effectiveDamage{ 0.0 };
        double effectiveArmor{ 0.0 };
    };

    struct CardinalTreeNodeViewModel
    {
        std::string id;
        std::string name;
        std::string description;
        NodeType nodeType{ NodeType::Form };
        NodeState state{ NodeState::Locked };
        double posX{ 0.0 };
        double posY{ 0.0 };
        std::vector<std::string> parentNodeIds;
        std::vector<std::string> requirementSummaries;
        std::vector<std::string> grantedAbilityNames;
    };

    struct FlatProgressionNodeViewModel
    {
        std::string id;
        std::string name;
        std::string description;
        NodeType nodeType{ NodeType::Form };
        NodeState state{ NodeState::Locked };
        int32_t gridX{ 0 };
        int32_t gridY{ 0 };
        std::vector<std::string> parentNodeIds;
        std::vector<std::string> requirementSummaries;
        std::vector<std::string> grantedAbilityNames;
    };

    struct FlatProgressionTreeViewModel
    {
        std::string seriesId;
        std::string seriesName;
        std::string weaponTypeId;
        std::vector<FlatProgressionNodeViewModel> nodes;
    };

    struct AbilitySummaryViewModel
    {
        std::string id;
        std::string name;
        std::string description;
        AbilityActivationType activationType{ AbilityActivationType::Passive };
        AbilityScope scope{ AbilityScope::Permanent };
        bool unlocked{ false };
    };

    struct StandardizedInfoPanelViewModel
    {
        // Section 1: Overview Header
        DashboardViewModel header;

        // Section 2: Selected Node Metadata
        std::string selectedNodeId;
        std::string selectedNodeName;
        std::string selectedNodeDescription;
        NodeType selectedNodeType{ NodeType::Form };
        NodeState selectedNodeState{ NodeState::Locked };
        bool canUnlock{ false };

        // Section 3: Requirement Progress List
        std::vector<std::string> requirementSummaries;

        // Section 4: Granted Abilities List
        std::vector<AbilitySummaryViewModel> grantedAbilities;

        // Section 5: Interaction Footer Hints
        std::string unlockButtonHint{ "[E] Unlock Node" };
        std::string backButtonHint{ "[Tab] Back" };
    };

    struct SkyUIIntegrationConfig
    {
        bool skyUIRequired{ true };
        std::string skyUIVersion{ "5.2 SE+" };
        std::string menuName{ "CardinalWeaponsMenu" };
        std::string swfPath{ "Interface/CardinalWeaponsMenu.swf" };
        bool enableFlat2DMode{ true };
        double leftPaneWidthPercent{ 55.0 };
        double rightPaneWidthPercent{ 45.0 };
    };

    /**
     * @brief Custom Cardinal Tree UI Framework Manager.
     * Manages data model feeds for the Flat 2D SkyUI Cardinal Tree menu (left tree, right info panel),
     * node selection previews, and standardized weapon templates.
     */
    class CardinalTreeUIManager
    {
    public:
        static CardinalTreeUIManager& GetInstance()
        {
            static CardinalTreeUIManager instance;
            return instance;
        }

        // Dashboard Data View
        [[nodiscard]] DashboardViewModel GetDashboardView() const;

        // Tree Network Data Views
        [[nodiscard]] std::vector<CardinalTreeNodeViewModel> GetTreeNodesForSeries(const std::string& seriesId) const;
        [[nodiscard]] FlatProgressionTreeViewModel GetFlatProgressionTreeForSeries(const std::string& seriesId) const;

        // Interactive Selection & Details Panel Views
        [[nodiscard]] CardinalTreeNodeViewModel GetNodeDetailsView(const std::string& nodeId) const;
        [[nodiscard]] StandardizedInfoPanelViewModel GetStandardizedInfoPanel(const std::string& nodeId) const;

        // SkyUI Configuration Access
        [[nodiscard]] SkyUIIntegrationConfig GetSkyUIConfig() const;

        // Open/Close UI State
        void OpenCardinalTreeMenu();
        void CloseCardinalTreeMenu();
        [[nodiscard]] bool IsMenuOpen() const;

        // Selection Trigger
        bool SelectAndUnlockNode(const std::string& nodeId);

    private:
        CardinalTreeUIManager() = default;
        ~CardinalTreeUIManager() = default;
        CardinalTreeUIManager(const CardinalTreeUIManager&) = delete;
        CardinalTreeUIManager& operator=(const CardinalTreeUIManager&) = delete;

        mutable std::shared_mutex m_mutex;
        bool m_isMenuOpen{ false };
    };
}
