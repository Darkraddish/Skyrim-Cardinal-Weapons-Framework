#include "CWFramework/ConstellationUIManager.h"
#include <mutex>

namespace CWFramework
{
    DashboardViewModel ConstellationUIManager::GetDashboardView() const
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

    std::vector<ConstellationNodeViewModel> ConstellationUIManager::GetConstellationNodesForSeries(const std::string& seriesId) const
    {
        std::vector<ConstellationNodeViewModel> result;
        auto nodes = Registry::GetInstance().GetNodesForSeries(seriesId);

        for (const auto& nData : nodes)
        {
            result.push_back(GetNodeDetailsView(nData.id));
        }

        return result;
    }

    ConstellationNodeViewModel ConstellationUIManager::GetNodeDetailsView(const std::string& nodeId) const
    {
        ConstellationNodeViewModel view;
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

    void ConstellationUIManager::OpenConstellationMenu()
    {
        std::unique_lock lock(m_mutex);
        m_isMenuOpen = true;
    }

    void ConstellationUIManager::CloseConstellationMenu()
    {
        std::unique_lock lock(m_mutex);
        m_isMenuOpen = false;
    }

    bool ConstellationUIManager::IsMenuOpen() const
    {
        std::shared_lock lock(m_mutex);
        return m_isMenuOpen;
    }

    bool ConstellationUIManager::SelectAndUnlockNode(const std::string& nodeId)
    {
        return ProgressionEngine::GetInstance().UnlockProgressionNode(nodeId);
    }
}
