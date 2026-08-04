#include "CWFramework/RequirementEvaluator.h"
#include <mutex>

namespace CWFramework
{
    bool RequirementEvaluator::IsRequirementMet(const RequirementData& requirement) const
    {
        auto [current, target] = GetRequirementProgress(requirement);
        return current >= target;
    }

    std::tuple<double, double> RequirementEvaluator::GetRequirementProgress(const RequirementData& requirement) const
    {
        std::shared_lock lock(m_mutex);
        double target = requirement.targetValue;

        switch (requirement.type)
        {
        case RequirementType::KillCount:
        {
            auto it = m_killCounts.find(requirement.targetId);
            double current = (it != m_killCounts.end()) ? static_cast<double>(it->second) : 0.0;
            return { current, target };
        }
        case RequirementType::ItemAbsorption:
        {
            auto it = m_itemAbsorbedCounts.find(requirement.targetId);
            double current = (it != m_itemAbsorbedCounts.end()) ? static_cast<double>(it->second) : 0.0;
            return { current, target };
        }
        case RequirementType::WeaponCopy:
        {
            auto it = m_copiedWeapons.find(requirement.targetId);
            double current = (it != m_copiedWeapons.end() && it->second) ? 1.0 : 0.0;
            return { current, 1.0 };
        }
        case RequirementType::QuestState:
        {
            auto it = m_questStages.find(requirement.targetId);
            double current = (it != m_questStages.end()) ? static_cast<double>(it->second) : 0.0;
            return { current, target };
        }
        case RequirementType::PlayerLevel:
        {
            return { static_cast<double>(m_playerLevel), target };
        }
        case RequirementType::ParentNodeDependency:
        {
            bool isParentUnlocked = SaveManager::GetInstance().IsNodeUnlocked(requirement.targetId);
            return { isParentUnlocked ? 1.0 : 0.0, 1.0 };
        }
        case RequirementType::SpecialEventTrigger:
        {
            auto it = m_triggeredEvents.find(requirement.targetId);
            double current = (it != m_triggeredEvents.end() && it->second) ? 1.0 : 0.0;
            return { current, 1.0 };
        }
        default:
            return { 0.0, target > 0.0 ? target : 1.0 };
        }
    }

    bool RequirementEvaluator::CanUnlockNode(const ProgressionNodeData& node) const
    {
        // 1. Parent Node Dependency Check
        for (const auto& parentId : node.parentNodeIds)
        {
            if (!SaveManager::GetInstance().IsNodeUnlocked(parentId))
            {
                return false; // Cannot unlock if parent node is incomplete!
            }
        }

        // 2. Requirements Check
        for (const auto& reqId : node.requirementIds)
        {
            auto reqOpt = Registry::GetInstance().GetRequirement(reqId);
            if (reqOpt.has_value())
            {
                if (!IsRequirementMet(reqOpt.value()))
                {
                    return false;
                }
            }
        }

        return true;
    }

    bool RequirementEvaluator::CanUnlockNodeById(const std::string& nodeId) const
    {
        auto nodeOpt = Registry::GetInstance().GetProgressionNode(nodeId);
        if (!nodeOpt.has_value()) return false;
        return CanUnlockNode(nodeOpt.value());
    }

    ProgressTuple RequirementEvaluator::GetProgressDetails(const RequirementData& requirement) const
    {
        auto [cur, tar] = GetRequirementProgress(requirement);
        return { cur, tar, cur >= tar };
    }

    void RequirementEvaluator::RecordKill(const std::string& actorBaseId, uint32_t count)
    {
        if (actorBaseId.empty()) return;
        std::unique_lock lock(m_mutex);
        m_killCounts[actorBaseId] += count;
    }

    void RequirementEvaluator::RecordItemAbsorption(const std::string& itemFormId, uint32_t count)
    {
        if (itemFormId.empty()) return;
        std::unique_lock lock(m_mutex);
        m_itemAbsorbedCounts[itemFormId] += count;
    }

    void RequirementEvaluator::RecordWeaponCopy(const std::string& weaponFormId)
    {
        if (weaponFormId.empty()) return;
        std::unique_lock lock(m_mutex);
        m_copiedWeapons[weaponFormId] = true;
    }

    void RequirementEvaluator::RecordQuestStage(const std::string& questId, uint32_t stage)
    {
        if (questId.empty()) return;
        std::unique_lock lock(m_mutex);
        m_questStages[questId] = stage;
    }

    void RequirementEvaluator::RecordPlayerLevel(uint32_t level)
    {
        std::unique_lock lock(m_mutex);
        m_playerLevel = level;
    }

    void RequirementEvaluator::TriggerSpecialEvent(const std::string& eventId)
    {
        if (eventId.empty()) return;
        std::unique_lock lock(m_mutex);
        m_triggeredEvents[eventId] = true;
    }

    uint32_t RequirementEvaluator::GetKillCount(const std::string& actorBaseId) const
    {
        std::shared_lock lock(m_mutex);
        auto it = m_killCounts.find(actorBaseId);
        return (it != m_killCounts.end()) ? it->second : 0;
    }

    uint32_t RequirementEvaluator::GetItemAbsorbedCount(const std::string& itemFormId) const
    {
        std::shared_lock lock(m_mutex);
        auto it = m_itemAbsorbedCounts.find(itemFormId);
        return (it != m_itemAbsorbedCounts.end()) ? it->second : 0;
    }

    bool RequirementEvaluator::IsWeaponCopied(const std::string& weaponFormId) const
    {
        std::shared_lock lock(m_mutex);
        auto it = m_copiedWeapons.find(weaponFormId);
        return (it != m_copiedWeapons.end()) ? it->second : false;
    }

    uint32_t RequirementEvaluator::GetQuestStage(const std::string& questId) const
    {
        std::shared_lock lock(m_mutex);
        auto it = m_questStages.find(questId);
        return (it != m_questStages.end()) ? it->second : 0;
    }

    uint32_t RequirementEvaluator::GetPlayerLevel() const
    {
        std::shared_lock lock(m_mutex);
        return m_playerLevel;
    }

    bool RequirementEvaluator::IsSpecialEventTriggered(const std::string& eventId) const
    {
        std::shared_lock lock(m_mutex);
        auto it = m_triggeredEvents.find(eventId);
        return (it != m_triggeredEvents.end()) ? it->second : false;
    }

    void RequirementEvaluator::ResetState()
    {
        std::unique_lock lock(m_mutex);
        m_killCounts.clear();
        m_itemAbsorbedCounts.clear();
        m_copiedWeapons.clear();
        m_questStages.clear();
        m_triggeredEvents.clear();
        m_playerLevel = 1;
    }
}
