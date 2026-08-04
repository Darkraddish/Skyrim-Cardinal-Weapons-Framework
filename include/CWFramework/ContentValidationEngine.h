#pragma once

#include "DataModels.h"
#include "Registry.h"
#include "SchemaValidator.h"
#include <shared_mutex>
#include <string>
#include <vector>
#include <unordered_map>

namespace CWFramework
{
    /**
     * @brief Content Validation Engine.
     * Implements Two-Stage Content Validation (Stage 1 Load-Time Structural Check & Stage 2 Pre-Unlock Verification)
     * with graceful degradation and soft-failure isolation for missing or broken content packs.
     */
    class ContentValidationEngine
    {
    public:
        static ContentValidationEngine& GetInstance()
        {
            static ContentValidationEngine instance;
            return instance;
        }

        // Stage 1: Load-Time Structural Validation
        [[nodiscard]] ValidationResult ValidateRegisteredContentPackData() const;

        // Stage 2: Pre-Unlock Runtime Verification
        [[nodiscard]] ValidationResult VerifyNodePreUnlock(const std::string& nodeId) const;
        [[nodiscard]] ValidationResult VerifyWeaponChoicePreSelection(const std::string& weaponTypeId) const;

        // Soft-Failure Management (Disables broken/missing items without crashing)
        void MarkItemUnavailable(const std::string& itemId, const std::string& reason);
        [[nodiscard]] bool IsItemAvailable(const std::string& itemId) const;
        [[nodiscard]] std::string GetUnavailableReason(const std::string& itemId) const;

        // Reset state
        void ResetState();

    private:
        ContentValidationEngine() = default;
        ~ContentValidationEngine() = default;
        ContentValidationEngine(const ContentValidationEngine&) = delete;
        ContentValidationEngine& operator=(const ContentValidationEngine&) = delete;

        mutable std::shared_mutex m_mutex;
        std::unordered_map<std::string, std::string> m_unavailableItems; // itemId -> reason
    };
}
