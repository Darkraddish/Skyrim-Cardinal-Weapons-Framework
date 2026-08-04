#pragma once

#include "DataModels.h"
#include "Registry.h"
#include "SaveManager.h"
#include "RequirementEvaluator.h"
#include "LegendaryWeaponManager.h"
#include <shared_mutex>
#include <string>
#include <vector>

namespace CWFramework
{
    /**
     * @brief Combined Progression & Sub-System Engines.
     * Manages Evolution, Level, Refinement, and Mastery independently (never unified into a single number).
     */
    class ProgressionEngine
    {
    public:
        static ProgressionEngine& GetInstance()
        {
            static ProgressionEngine instance;
            return instance;
        }

        // --- 1. EVOLUTION ENGINE (Web Traversal & Convertible Nodes) ---
        bool UnlockProgressionNode(const std::string& nodeId);
        bool ResolveConvertibleNode(const std::string& nodeId, const std::string& outcomeFormId);
        [[nodiscard]] NodeState GetNodeState(const std::string& nodeId) const;
        void UpdateAllNodeStatesForSeries(const std::string& seriesId);

        // --- 2. LEVEL ENGINE (Flat Additive Growth) ---
        void AddFlatLevelBonus(double bonus);
        [[nodiscard]] double GetLevelBonus() const;

        // --- 3. REFINEMENT ENGINE (Multiplicative Crafting Quality) ---
        bool UpgradeRefinementTier();
        [[nodiscard]] uint32_t GetRefinementLevel() const;
        [[nodiscard]] double GetRefinementMultiplier() const;

        // --- 4. MASTERY ENGINE (Passive Usage Multiplier) ---
        void AddMasteryXp(double xpAmount);
        [[nodiscard]] double GetMasteryXp() const;
        [[nodiscard]] double GetMasteryMultiplier() const;

        // Combined Weapon Power Stat Calculation
        [[nodiscard]] double CalculateEffectiveDamage(double baseDamage) const;
        [[nodiscard]] double CalculateEffectiveArmor(double baseArmor) const;

        // Reset state
        void ResetState();

    private:
        ProgressionEngine() = default;
        ~ProgressionEngine() = default;
        ProgressionEngine(const ProgressionEngine&) = delete;
        ProgressionEngine& operator=(const ProgressionEngine&) = delete;

        mutable std::shared_mutex m_mutex;
    };
}
