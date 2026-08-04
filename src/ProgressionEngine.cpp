#include "CWFramework/ProgressionEngine.h"
#include <cmath>
#include <mutex>

namespace CWFramework
{
    // --- 1. EVOLUTION ENGINE ---
    bool ProgressionEngine::UnlockProgressionNode(const std::string& nodeId)
    {
        if (nodeId.empty()) return false;

        auto nodeOpt = Registry::GetInstance().GetProgressionNode(nodeId);
        if (!nodeOpt.has_value()) return false;
        const auto& nodeData = nodeOpt.value();

        // Must satisfy parent dependencies and requirements
        if (!RequirementEvaluator::GetInstance().CanUnlockNode(nodeData))
        {
            return false;
        }

        // Unlock in SaveManager
        SaveManager::GetInstance().UnlockNode(nodeId);

        // If node unlocks a new form, evolve active form
        if (!nodeData.unlockedFormId.empty())
        {
            LegendaryWeaponManager::GetInstance().SwapActiveForm(nodeData.unlockedFormId);
        }

        // Reveal branch if event node specifies revealed branches
        for (const auto& seriesId : nodeData.revealedBranchSeriesIds)
        {
            SaveManager::GetInstance().RevealSeriesBranch(seriesId);
        }

        // Refresh adjacent node states in series tree
        UpdateAllNodeStatesForSeries(nodeData.seriesId);
        return true;
    }

    bool ProgressionEngine::ResolveConvertibleNode(const std::string& nodeId, const std::string& outcomeFormId)
    {
        if (nodeId.empty() || outcomeFormId.empty()) return false;

        SaveManager::GetInstance().ResolveConvertibleNode(nodeId, outcomeFormId);
        // Swaps current active form to convertible outcome
        LegendaryWeaponManager::GetInstance().SwapActiveForm(outcomeFormId);
        return true;
    }

    NodeState ProgressionEngine::GetNodeState(const std::string& nodeId) const
    {
        if (SaveManager::GetInstance().IsNodeUnlocked(nodeId))
        {
            return NodeState::Unlocked;
        }
        if (RequirementEvaluator::GetInstance().CanUnlockNodeById(nodeId))
        {
            return NodeState::Unlockable;
        }
        return NodeState::Locked;
    }

    void ProgressionEngine::UpdateAllNodeStatesForSeries(const std::string& seriesId)
    {
        auto nodes = Registry::GetInstance().GetNodesForSeries(seriesId);
        for (const auto& node : nodes)
        {
            NodeState st = GetNodeState(node.id);
            SaveManager::GetInstance().SetNodeState(node.id, st);
        }
    }

    // --- 2. LEVEL ENGINE (Flat Additive Growth) ---
    void ProgressionEngine::AddFlatLevelBonus(double bonus)
    {
        auto state = SaveManager::GetInstance().GetSubSystemsState();
        state.levelBonus += bonus;
        SaveManager::GetInstance().UpdateSubSystemsState(state);
    }

    double ProgressionEngine::GetLevelBonus() const
    {
        return SaveManager::GetInstance().GetSubSystemsState().levelBonus;
    }

    // --- 3. REFINEMENT ENGINE (Multiplicative Crafting Quality) ---
    bool ProgressionEngine::UpgradeRefinementTier()
    {
        auto state = SaveManager::GetInstance().GetSubSystemsState();
        state.refinementLevel += 1;
        SaveManager::GetInstance().UpdateSubSystemsState(state);
        return true;
    }

    uint32_t ProgressionEngine::GetRefinementLevel() const
    {
        return SaveManager::GetInstance().GetSubSystemsState().refinementLevel;
    }

    double ProgressionEngine::GetRefinementMultiplier() const
    {
        uint32_t level = GetRefinementLevel();
        // Each refinement tier adds +15% multiplicative quality (1.0, 1.15, 1.30, 1.45...)
        return 1.0 + (static_cast<double>(level) * 0.15);
    }

    // --- 4. MASTERY ENGINE (Passive Continuous Usage Multiplier) ---
    void ProgressionEngine::AddMasteryXp(double xpAmount)
    {
        auto state = SaveManager::GetInstance().GetSubSystemsState();
        state.masteryXp += xpAmount;
        // Formula: masteryMultiplier = 1.0 + log10(1.0 + masteryXp / 100.0) * 0.5
        state.masteryMultiplier = 1.0 + std::log10(1.0 + state.masteryXp / 100.0) * 0.5;
        SaveManager::GetInstance().UpdateSubSystemsState(state);
    }

    double ProgressionEngine::GetMasteryXp() const
    {
        return SaveManager::GetInstance().GetSubSystemsState().masteryXp;
    }

    double ProgressionEngine::GetMasteryMultiplier() const
    {
        return SaveManager::GetInstance().GetSubSystemsState().masteryMultiplier;
    }

    // --- COMBINED STAT CALCULATION ENGINE ---
    // Formula: Effective = (BaseStat + LevelBonus) * RefinementMultiplier * MasteryMultiplier
    double ProgressionEngine::CalculateEffectiveDamage(double baseDamage) const
    {
        double levelBonus = GetLevelBonus();
        double refMult = GetRefinementMultiplier();
        double masMult = GetMasteryMultiplier();
        return (baseDamage + levelBonus) * refMult * masMult;
    }

    double ProgressionEngine::CalculateEffectiveArmor(double baseArmor) const
    {
        double levelBonus = GetLevelBonus();
        double refMult = GetRefinementMultiplier();
        double masMult = GetMasteryMultiplier();
        return (baseArmor + levelBonus) * refMult * masMult;
    }

    void ProgressionEngine::ResetState()
    {
        SaveManager::GetInstance().ResetSaveState();
    }
}
