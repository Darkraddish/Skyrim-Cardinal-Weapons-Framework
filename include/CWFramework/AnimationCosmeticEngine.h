#pragma once

#include "DataModels.h"
#include "Registry.h"
#include "SaveManager.h"
#include <shared_mutex>
#include <string>
#include <vector>

namespace CWFramework
{
    struct TransformationEffectEvent
    {
        std::string formId;
        std::string visualEffectId;
        std::string soundEffectId;
    };

    /**
     * @brief Animation & Cosmetic Integration Engine.
     * Provides OAR/DAR animation condition variables and cosmetic transformation particle/sound triggers.
     */
    class AnimationCosmeticEngine
    {
    public:
        static AnimationCosmeticEngine& GetInstance()
        {
            static AnimationCosmeticEngine instance;
            return instance;
        }

        // OAR / DAR Condition Variable Export APIs
        [[nodiscard]] std::string GetOARWeaponFormConditionVariable() const;
        [[nodiscard]] std::string GetOARWeaponTypeConditionVariable() const;

        // Visual & Sound Transformation Effect Trigger System
        void TriggerTransformationEffects(const std::string& newFormId);

        [[nodiscard]] std::vector<TransformationEffectEvent> GetEffectHistory() const;

        // Reset state
        void ResetState();

    private:
        AnimationCosmeticEngine() = default;
        ~AnimationCosmeticEngine() = default;
        AnimationCosmeticEngine(const AnimationCosmeticEngine&) = delete;
        AnimationCosmeticEngine& operator=(const AnimationCosmeticEngine&) = delete;

        mutable std::shared_mutex m_mutex;
        std::vector<TransformationEffectEvent> m_effectHistory;
    };
}
