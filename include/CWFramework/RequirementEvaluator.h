#pragma once

#include "Interfaces.h"
#include "Registry.h"
#include "SaveManager.h"
#include <shared_mutex>
#include <string>
#include <unordered_map>
#include <tuple>

namespace CWFramework
{
    struct ProgressTuple
    {
        double current{ 0.0 };
        double target{ 0.0 };
        bool isMet{ false };
    };

    /**
     * @brief Requirement Engine implementation.
     * Evaluates generic unlock conditions, tracks live progress, and enforces parent node dependencies.
     */
    class RequirementEvaluator : public IRequirementEvaluator
    {
    public:
        static RequirementEvaluator& GetInstance()
        {
            static RequirementEvaluator instance;
            return instance;
        }

        // IRequirementEvaluator interface implementation
        [[nodiscard]] bool IsRequirementMet(const RequirementData& requirement) const override;
        [[nodiscard]] std::tuple<double, double> GetRequirementProgress(const RequirementData& requirement) const override;

        // Node Unlock Availability (Parent node dependencies + all requirements)
        [[nodiscard]] bool CanUnlockNode(const ProgressionNodeData& node) const;
        [[nodiscard]] bool CanUnlockNodeById(const std::string& nodeId) const;

        // Live Progress tuple helper
        [[nodiscard]] ProgressTuple GetProgressDetails(const RequirementData& requirement) const;

        // Progress Tracking & Event Registration APIs
        void RecordKill(const std::string& actorBaseId, uint32_t count = 1);
        void RecordItemAbsorption(const std::string& itemFormId, uint32_t count = 1);
        void RecordWeaponCopy(const std::string& weaponFormId);
        void RecordQuestStage(const std::string& questId, uint32_t stage);
        void RecordPlayerLevel(uint32_t level);
        void TriggerSpecialEvent(const std::string& eventId);

        [[nodiscard]] uint32_t GetKillCount(const std::string& actorBaseId) const;
        [[nodiscard]] uint32_t GetItemAbsorbedCount(const std::string& itemFormId) const;
        [[nodiscard]] bool IsWeaponCopied(const std::string& weaponFormId) const;
        [[nodiscard]] uint32_t GetQuestStage(const std::string& questId) const;
        [[nodiscard]] uint32_t GetPlayerLevel() const;
        [[nodiscard]] bool IsSpecialEventTriggered(const std::string& eventId) const;

        // Reset state (for save load / testing)
        void ResetState();

    private:
        RequirementEvaluator() = default;
        ~RequirementEvaluator() override = default;
        RequirementEvaluator(const RequirementEvaluator&) = delete;
        RequirementEvaluator& operator=(const RequirementEvaluator&) = delete;

        mutable std::shared_mutex m_mutex;

        std::unordered_map<std::string, uint32_t> m_killCounts;
        std::unordered_map<std::string, uint32_t> m_itemAbsorbedCounts;
        std::unordered_map<std::string, bool> m_copiedWeapons;
        std::unordered_map<std::string, uint32_t> m_questStages;
        std::unordered_map<std::string, bool> m_triggeredEvents;
        uint32_t m_playerLevel{ 1 };
    };
}
