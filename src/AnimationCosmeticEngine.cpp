#include "CWFramework/AnimationCosmeticEngine.h"
#include <mutex>

namespace CWFramework
{
    std::string AnimationCosmeticEngine::GetOARWeaponFormConditionVariable() const
    {
        return SaveManager::GetInstance().GetCurrentFormId();
    }

    std::string AnimationCosmeticEngine::GetOARWeaponTypeConditionVariable() const
    {
        return SaveManager::GetInstance().GetChosenWeaponTypeId();
    }

    void AnimationCosmeticEngine::TriggerTransformationEffects(const std::string& newFormId)
    {
        if (newFormId.empty()) return;

        std::unique_lock lock(m_mutex);

        TransformationEffectEvent event;
        event.formId = newFormId;
        event.visualEffectId = "FX_CardinalTransformation_Glow";
        event.soundEffectId = "SND_CardinalEvolution_Chime";

        m_effectHistory.push_back(event);
    }

    std::vector<TransformationEffectEvent> AnimationCosmeticEngine::GetEffectHistory() const
    {
        std::shared_lock lock(m_mutex);
        return m_effectHistory;
    }

    void AnimationCosmeticEngine::ResetState()
    {
        std::unique_lock lock(m_mutex);
        m_effectHistory.clear();
    }
}
