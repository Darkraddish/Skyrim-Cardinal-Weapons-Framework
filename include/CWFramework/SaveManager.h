#pragma once

#include "SaveData.h"
#include "SchemaValidator.h"
#include <shared_mutex>
#include <memory>
#include <string>
#include <vector>

namespace CWFramework
{
    /**
     * @brief Save & Persistence Engine Manager.
     * Manages character save lifetime state and enforces single-weapon lock rule.
     */
    class SaveManager
    {
    public:
        static SaveManager& GetInstance()
        {
            static SaveManager instance;
            return instance;
        }

        // Weapon Choice Lock-In (Enforces EXACTLY ONE Legendary Weapon per character save)
        bool LockInWeaponChoice(const std::string& weaponTypeId);
        [[nodiscard]] bool IsWeaponLocked() const;
        [[nodiscard]] std::string GetChosenWeaponTypeId() const;

        // Weapon Form & Progression State
        bool SetCurrentForm(const std::string& formId);
        [[nodiscard]] std::string GetCurrentFormId() const;

        bool UnlockNode(const std::string& nodeId);
        [[nodiscard]] bool IsNodeUnlocked(const std::string& nodeId) const;
        [[nodiscard]] NodeState GetNodeState(const std::string& nodeId) const;
        void SetNodeState(const std::string& nodeId, NodeState state);

        // Branch Visibility & Convertible Node Resolution
        void RevealSeriesBranch(const std::string& seriesId);
        [[nodiscard]] bool IsSeriesBranchRevealed(const std::string& seriesId) const;

        void ResolveConvertibleNode(const std::string& nodeId, const std::string& outcomeFormId);
        [[nodiscard]] std::string GetConvertibleNodeResolution(const std::string& nodeId) const;

        // Sub-Systems State Access
        [[nodiscard]] SubSystemsState GetSubSystemsState() const;
        void UpdateSubSystemsState(const SubSystemsState& state);

        // Serialization & Validation
        void Serialize(std::ostream& os) const;
        bool Deserialize(std::istream& is);
        [[nodiscard]] ValidationResult ValidateSaveState() const;

        // Reset state for new character / save load
        void ResetSaveState();

        // Direct SaveData copy query for inspection
        [[nodiscard]] SaveData GetSaveDataCopy() const;

    private:
        SaveManager() = default;
        ~SaveManager() = default;
        SaveManager(const SaveManager&) = delete;
        SaveManager& operator=(const SaveManager&) = delete;

        mutable std::shared_mutex m_mutex;
        SaveData m_saveData;
    };
}
