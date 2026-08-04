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

    struct ConstellationNodeViewModel
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

    /**
     * @brief Custom Constellation UI Framework Manager.
     * Manages data model feeds for the standalone radial constellation perk menu, node selection previews, and dashboards.
     */
    class ConstellationUIManager
    {
    public:
        static ConstellationUIManager& GetInstance()
        {
            static ConstellationUIManager instance;
            return instance;
        }

        // Dashboard Data View
        [[nodiscard]] DashboardViewModel GetDashboardView() const;

        // Constellation Network Data View
        [[nodiscard]] std::vector<ConstellationNodeViewModel> GetConstellationNodesForSeries(const std::string& seriesId) const;

        // Interactive Selection & Details Panel Views
        [[nodiscard]] ConstellationNodeViewModel GetNodeDetailsView(const std::string& nodeId) const;

        // Open/Close UI State
        void OpenConstellationMenu();
        void CloseConstellationMenu();
        [[nodiscard]] bool IsMenuOpen() const;

        // Selection Trigger
        bool SelectAndUnlockNode(const std::string& nodeId);

    private:
        ConstellationUIManager() = default;
        ~ConstellationUIManager() = default;
        ConstellationUIManager(const ConstellationUIManager&) = delete;
        ConstellationUIManager& operator=(const ConstellationUIManager&) = delete;

        mutable std::shared_mutex m_mutex;
        bool m_isMenuOpen{ false };
    };
}
