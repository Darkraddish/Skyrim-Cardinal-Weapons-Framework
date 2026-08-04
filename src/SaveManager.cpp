#include "CWFramework/SaveManager.h"
#include <algorithm>
#include <mutex>

namespace CWFramework
{
    bool SaveManager::LockInWeaponChoice(const std::string& weaponTypeId)
    {
        if (weaponTypeId.empty()) return false;
        std::unique_lock lock(m_mutex);

        // Strict Rule: Exactly one Legendary Weapon per save. If already locked, REJECT!
        if (m_saveData.isWeaponLocked)
        {
            return false;
        }

        m_saveData.chosenWeaponTypeId = weaponTypeId;
        m_saveData.isWeaponLocked = true;
        return true;
    }

    bool SaveManager::IsWeaponLocked() const
    {
        std::shared_lock lock(m_mutex);
        return m_saveData.isWeaponLocked;
    }

    std::string SaveManager::GetChosenWeaponTypeId() const
    {
        std::shared_lock lock(m_mutex);
        return m_saveData.chosenWeaponTypeId;
    }

    bool SaveManager::SetCurrentForm(const std::string& formId)
    {
        std::unique_lock lock(m_mutex);
        m_saveData.currentFormId = formId;
        return true;
    }

    std::string SaveManager::GetCurrentFormId() const
    {
        std::shared_lock lock(m_mutex);
        return m_saveData.currentFormId;
    }

    bool SaveManager::UnlockNode(const std::string& nodeId)
    {
        if (nodeId.empty()) return false;
        std::unique_lock lock(m_mutex);

        auto it = std::find(m_saveData.unlockedNodeIds.begin(), m_saveData.unlockedNodeIds.end(), nodeId);
        if (it == m_saveData.unlockedNodeIds.end())
        {
            m_saveData.unlockedNodeIds.push_back(nodeId);
        }
        m_saveData.nodeStates[nodeId] = static_cast<uint8_t>(NodeState::Unlocked);
        return true;
    }

    bool SaveManager::IsNodeUnlocked(const std::string& nodeId) const
    {
        std::shared_lock lock(m_mutex);
        auto it = std::find(m_saveData.unlockedNodeIds.begin(), m_saveData.unlockedNodeIds.end(), nodeId);
        return it != m_saveData.unlockedNodeIds.end();
    }

    NodeState SaveManager::GetNodeState(const std::string& nodeId) const
    {
        std::shared_lock lock(m_mutex);
        auto it = m_saveData.nodeStates.find(nodeId);
        if (it != m_saveData.nodeStates.end())
        {
            return static_cast<NodeState>(it->second);
        }
        return NodeState::Locked;
    }

    void SaveManager::SetNodeState(const std::string& nodeId, NodeState state)
    {
        std::unique_lock lock(m_mutex);
        m_saveData.nodeStates[nodeId] = static_cast<uint8_t>(state);
    }

    void SaveManager::RevealSeriesBranch(const std::string& seriesId)
    {
        if (seriesId.empty()) return;
        std::unique_lock lock(m_mutex);
        auto it = std::find(m_saveData.revealedBranchSeriesIds.begin(), m_saveData.revealedBranchSeriesIds.end(), seriesId);
        if (it == m_saveData.revealedBranchSeriesIds.end())
        {
            m_saveData.revealedBranchSeriesIds.push_back(seriesId);
        }
    }

    bool SaveManager::IsSeriesBranchRevealed(const std::string& seriesId) const
    {
        std::shared_lock lock(m_mutex);
        auto it = std::find(m_saveData.revealedBranchSeriesIds.begin(), m_saveData.revealedBranchSeriesIds.end(), seriesId);
        return it != m_saveData.revealedBranchSeriesIds.end();
    }

    void SaveManager::ResolveConvertibleNode(const std::string& nodeId, const std::string& outcomeFormId)
    {
        if (nodeId.empty()) return;
        std::unique_lock lock(m_mutex);
        m_saveData.convertibleNodeResolutions[nodeId] = outcomeFormId;
    }

    std::string SaveManager::GetConvertibleNodeResolution(const std::string& nodeId) const
    {
        std::shared_lock lock(m_mutex);
        auto it = m_saveData.convertibleNodeResolutions.find(nodeId);
        if (it != m_saveData.convertibleNodeResolutions.end())
        {
            return it->second;
        }
        return {};
    }

    SubSystemsState SaveManager::GetSubSystemsState() const
    {
        std::shared_lock lock(m_mutex);
        return m_saveData.subSystems;
    }

    void SaveManager::UpdateSubSystemsState(const SubSystemsState& state)
    {
        std::unique_lock lock(m_mutex);
        m_saveData.subSystems = state;
    }

    void SaveManager::Serialize(std::ostream& os) const
    {
        std::shared_lock lock(m_mutex);
        m_saveData.Serialize(os);
    }

    bool SaveManager::Deserialize(std::istream& is)
    {
        std::unique_lock lock(m_mutex);
        return m_saveData.Deserialize(is);
    }

    ValidationResult SaveManager::ValidateSaveState() const
    {
        std::shared_lock lock(m_mutex);
        ValidationResult res;

        // If locked flag is set but chosen weapon ID is empty
        if (m_saveData.isWeaponLocked && m_saveData.chosenWeaponTypeId.empty())
        {
            res.AddError("Save state invalid: Weapon choice is locked but chosenWeaponTypeId is empty.");
        }

        // If weapon is NOT locked but active form or unlocked nodes exist
        if (!m_saveData.isWeaponLocked)
        {
            if (!m_saveData.currentFormId.empty())
            {
                res.AddError("Save state invalid: Active weapon form exists before weapon choice is locked.");
            }
            if (!m_saveData.unlockedNodeIds.empty())
            {
                res.AddError("Save state invalid: Unlocked nodes exist before weapon choice is locked.");
            }
        }

        return res;
    }

    void SaveManager::ResetSaveState()
    {
        std::unique_lock lock(m_mutex);
        m_saveData.Clear();
    }

    SaveData SaveManager::GetSaveDataCopy() const
    {
        std::shared_lock lock(m_mutex);
        return m_saveData;
    }
}
