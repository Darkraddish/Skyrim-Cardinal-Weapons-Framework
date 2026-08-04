#include "CWFramework/CardinalTreeUIManager.h"
#include <mutex>

namespace CWFramework
{
    DashboardViewModel CardinalTreeUIManager::GetDashboardView() const
    {
        DashboardViewModel view;

        std::string wtId = SaveManager::GetInstance().GetChosenWeaponTypeId();
        auto wtOpt = Registry::GetInstance().GetWeaponType(wtId);
        view.chosenWeaponTypeName = wtOpt.has_value() ? wtOpt->name : "None";

        std::string formId = SaveManager::GetInstance().GetCurrentFormId();
        auto formOpt = Registry::GetInstance().GetWeaponForm(formId);
        if (formOpt.has_value())
        {
            view.currentFormName = formOpt->name;
            view.currentFormIconAsset = formOpt->iconAsset;

            auto serOpt = Registry::GetInstance().GetSeries(formOpt->seriesId);
            view.activeSeriesName = serOpt.has_value() ? serOpt->name : "None";

            view.effectiveDamage = ProgressionEngine::GetInstance().CalculateEffectiveDamage(formOpt->baseDamage);
            view.effectiveArmor = ProgressionEngine::GetInstance().CalculateEffectiveArmor(formOpt->baseArmor);
        }
        else
        {
            view.currentFormName = "Unchosen";
            view.activeSeriesName = "None";
            view.effectiveDamage = 0.0;
            view.effectiveArmor = 0.0;
        }

        view.levelBonus = ProgressionEngine::GetInstance().GetLevelBonus();
        view.refinementLevel = ProgressionEngine::GetInstance().GetRefinementLevel();
        view.masteryMultiplier = ProgressionEngine::GetInstance().GetMasteryMultiplier();

        return view;
    }

    std::vector<CardinalTreeNodeViewModel> CardinalTreeUIManager::GetTreeNodesForSeries(const std::string& seriesId) const
    {
        std::vector<CardinalTreeNodeViewModel> result;
        auto nodes = Registry::GetInstance().GetNodesForSeries(seriesId);

        for (const auto& nData : nodes)
        {
            result.push_back(GetNodeDetailsView(nData.id));
        }

        return result;
    }

    CardinalTreeNodeViewModel CardinalTreeUIManager::GetNodeDetailsView(const std::string& nodeId) const
    {
        CardinalTreeNodeViewModel view;
        auto nodeOpt = Registry::GetInstance().GetProgressionNode(nodeId);
        if (!nodeOpt.has_value()) return view;

        const auto& nData = nodeOpt.value();
        view.id = nData.id;
        view.name = nData.name;
        view.description = nData.description;
        view.nodeType = nData.nodeType;
        view.posX = nData.posX;
        view.posY = nData.posY;
        view.parentNodeIds = nData.parentNodeIds;
        view.state = ProgressionEngine::GetInstance().GetNodeState(nData.id);

        // Populate requirement summaries
        for (const auto& rId : nData.requirementIds)
        {
            auto reqOpt = Registry::GetInstance().GetRequirement(rId);
            if (reqOpt.has_value())
            {
                auto [cur, tar] = RequirementEvaluator::GetInstance().GetRequirementProgress(reqOpt.value());
                std::string summary = reqOpt->description + " (" + std::to_string(static_cast<int>(cur)) + "/" + std::to_string(static_cast<int>(tar)) + ")";
                view.requirementSummaries.push_back(summary);
            }
        }

        // Populate granted ability names
        for (const auto& aId : nData.abilityIds)
        {
            auto abiOpt = Registry::GetInstance().GetAbility(aId);
            if (abiOpt.has_value())
            {
                view.grantedAbilityNames.push_back(abiOpt->name);
            }
        }

        return view;
    }

    FlatProgressionTreeViewModel CardinalTreeUIManager::GetFlatProgressionTreeForSeries(const std::string& seriesId) const
    {
        FlatProgressionTreeViewModel treeView;
        treeView.seriesId = seriesId;

        auto serOpt = Registry::GetInstance().GetSeries(seriesId);
        if (serOpt.has_value())
        {
            treeView.seriesName = serOpt->name;
            treeView.weaponTypeId = serOpt->weaponTypeId;
        }

        auto nodes = Registry::GetInstance().GetNodesForSeries(seriesId);
        for (const auto& nData : nodes)
        {
            FlatProgressionNodeViewModel flatNode;
            flatNode.id = nData.id;
            flatNode.name = nData.name;
            flatNode.description = nData.description;
            flatNode.nodeType = nData.nodeType;
            flatNode.state = ProgressionEngine::GetInstance().GetNodeState(nData.id);
            flatNode.gridX = static_cast<int32_t>(nData.posX);
            flatNode.gridY = static_cast<int32_t>(nData.posY);
            flatNode.parentNodeIds = nData.parentNodeIds;

            auto details = GetNodeDetailsView(nData.id);
            flatNode.requirementSummaries = details.requirementSummaries;
            flatNode.grantedAbilityNames = details.grantedAbilityNames;

            treeView.nodes.push_back(flatNode);
        }

        return treeView;
    }

    StandardizedInfoPanelViewModel CardinalTreeUIManager::GetStandardizedInfoPanel(const std::string& nodeId) const
    {
        StandardizedInfoPanelViewModel panel;

        // Section 1: Weapon Overview Header
        panel.header = GetDashboardView();

        // Section 2: Selected Node Metadata
        auto nodeOpt = Registry::GetInstance().GetProgressionNode(nodeId);
        if (nodeOpt.has_value())
        {
            const auto& nData = nodeOpt.value();
            panel.selectedNodeId = nData.id;
            panel.selectedNodeName = nData.name;
            panel.selectedNodeDescription = nData.description;
            panel.selectedNodeType = nData.nodeType;
            panel.selectedNodeState = ProgressionEngine::GetInstance().GetNodeState(nData.id);
            panel.canUnlock = RequirementEvaluator::GetInstance().CanUnlockNode(nData);

            // Section 3: Unlock Requirements Tracker
            for (const auto& rId : nData.requirementIds)
            {
                auto reqOpt = Registry::GetInstance().GetRequirement(rId);
                if (reqOpt.has_value())
                {
                    auto [cur, tar] = RequirementEvaluator::GetInstance().GetRequirementProgress(reqOpt.value());
                    std::string summary = reqOpt->description + " (" + std::to_string(static_cast<int>(cur)) + "/" + std::to_string(static_cast<int>(tar)) + ")";
                    panel.requirementSummaries.push_back(summary);
                }
            }

            // Section 4: Granted Abilities List
            for (const auto& aId : nData.abilityIds)
            {
                auto abiOpt = Registry::GetInstance().GetAbility(aId);
                if (abiOpt.has_value())
                {
                    AbilitySummaryViewModel abiView;
                    abiView.id = abiOpt->id;
                    abiView.name = abiOpt->name;
                    abiView.description = abiOpt->description;
                    abiView.scope = abiOpt->scope;
                    abiView.unlocked = (panel.selectedNodeState == NodeState::Unlocked);
                    panel.grantedAbilities.push_back(abiView);
                }
            }
        }

        // Section 5: Interaction Footer Hints
        panel.unlockButtonHint = panel.canUnlock ? "[E] Unlock Node" : "[E] Locked (Requirements Incomplete)";
        panel.backButtonHint = "[Tab] Back";

        return panel;
    }

    SkyUIIntegrationConfig CardinalTreeUIManager::GetSkyUIConfig() const
    {
        SkyUIIntegrationConfig config;
        config.skyUIRequired = true;
        config.skyUIVersion = "5.2 SE+";
        config.menuName = "CardinalWeaponsMenu";
        config.swfPath = "Interface/CardinalWeaponsMenu.swf";
        config.enableFlat2DMode = true;
        config.leftPaneWidthPercent = 55.0;
        config.rightPaneWidthPercent = 45.0;
        return config;
    }

    void CardinalTreeUIManager::OpenCardinalTreeMenu()
    {
        std::unique_lock lock(m_mutex);
        m_isMenuOpen = true;
    }

    void CardinalTreeUIManager::CloseCardinalTreeMenu()
    {
        std::unique_lock lock(m_mutex);
        m_isMenuOpen = false;
    }

    bool CardinalTreeUIManager::IsMenuOpen() const
    {
        std::shared_lock lock(m_mutex);
        return m_isMenuOpen;
    }

    bool CardinalTreeUIManager::SelectAndUnlockNode(const std::string& nodeId)
    {
        return ProgressionEngine::GetInstance().UnlockProgressionNode(nodeId);
    }
}
