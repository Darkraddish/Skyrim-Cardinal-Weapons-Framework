#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <cstdint>
#include <iostream>
#include <sstream>

namespace CWFramework
{
    /**
     * @brief Node visual and availability state markers.
     */
    enum class NodeState : uint8_t
    {
        Locked = 0,
        Unlockable = 1,
        Unlocked = 2
    };

    /**
     * @brief Persistent growth states for the 4 independent growth sub-systems.
     */
    struct SubSystemsState
    {
        double levelBonus{ 0.0 };              // Flat additive stat growth
        uint32_t refinementLevel{ 0 };          // Multiplicative crafting enhancement quality tier
        double masteryXp{ 0.0 };               // Usage familiarity XP
        double masteryMultiplier{ 1.0 };       // Passive usage multiplier

        void Reset()
        {
            levelBonus = 0.0;
            refinementLevel = 0;
            masteryXp = 0.0;
            masteryMultiplier = 1.0;
        }
    };

    /**
     * @brief Persistent save data model for a player's character save file.
     */
    struct SaveData
    {
        uint32_t version{ 1 };
        bool isWeaponLocked{ false };
        std::string chosenWeaponTypeId;        // Permanent lock-in per character save
        std::string currentFormId;            // Active form (series derived implicitly via form)
        
        std::vector<std::string> unlockedNodeIds;
        std::unordered_map<std::string, std::string> convertibleNodeResolutions; // nodeId -> resolvedOutcomeFormId
        std::vector<std::string> revealedBranchSeriesIds;
        std::unordered_map<std::string, uint8_t> nodeStates; // nodeId -> NodeState value (0, 1, 2)
        
        SubSystemsState subSystems;

        void Clear()
        {
            version = 1;
            isWeaponLocked = false;
            chosenWeaponTypeId.clear();
            currentFormId.clear();
            unlockedNodeIds.clear();
            convertibleNodeResolutions.clear();
            revealedBranchSeriesIds.clear();
            nodeStates.clear();
            subSystems.Reset();
        }

        /**
         * @brief Serializes SaveData into a stream (SKSE CoSave format).
         */
        void Serialize(std::ostream& os) const
        {
            // Simple robust binary-equivalent text serialization stream
            os << version << "\n";
            os << (isWeaponLocked ? 1 : 0) << "\n";
            os << (chosenWeaponTypeId.empty() ? "<none>" : chosenWeaponTypeId) << "\n";
            os << (currentFormId.empty() ? "<none>" : currentFormId) << "\n";

            // Unlocked nodes
            os << unlockedNodeIds.size() << "\n";
            for (const auto& id : unlockedNodeIds)
            {
                os << id << "\n";
            }

            // Convertible resolutions
            os << convertibleNodeResolutions.size() << "\n";
            for (const auto& [nodeId, formId] : convertibleNodeResolutions)
            {
                os << nodeId << " " << formId << "\n";
            }

            // Revealed series branches
            os << revealedBranchSeriesIds.size() << "\n";
            for (const auto& sId : revealedBranchSeriesIds)
            {
                os << sId << "\n";
            }

            // Node states
            os << nodeStates.size() << "\n";
            for (const auto& [nodeId, stateVal] : nodeStates)
            {
                os << nodeId << " " << static_cast<uint32_t>(stateVal) << "\n";
            }

            // SubSystems
            os << subSystems.levelBonus << " "
               << subSystems.refinementLevel << " "
               << subSystems.masteryXp << " "
               << subSystems.masteryMultiplier << "\n";
        }

        /**
         * @brief Deserializes SaveData from a stream.
         */
        bool Deserialize(std::istream& is)
        {
            Clear();
            if (!(is >> version)) return false;
            
            uint32_t lockedVal = 0;
            if (!(is >> lockedVal)) return false;
            isWeaponLocked = (lockedVal != 0);

            if (!(is >> chosenWeaponTypeId)) return false;
            if (chosenWeaponTypeId == "<none>") chosenWeaponTypeId.clear();

            if (!(is >> currentFormId)) return false;
            if (currentFormId == "<none>") currentFormId.clear();

            // Unlocked nodes
            size_t unlockedCount = 0;
            if (!(is >> unlockedCount)) return false;
            unlockedNodeIds.reserve(unlockedCount);
            for (size_t i = 0; i < unlockedCount; ++i)
            {
                std::string id;
                if (is >> id) unlockedNodeIds.push_back(id);
            }

            // Convertible resolutions
            size_t convCount = 0;
            if (!(is >> convCount)) return false;
            for (size_t i = 0; i < convCount; ++i)
            {
                std::string nId, fId;
                if (is >> nId >> fId) convertibleNodeResolutions[nId] = fId;
            }

            // Revealed series branches
            size_t revCount = 0;
            if (!(is >> revCount)) return false;
            for (size_t i = 0; i < revCount; ++i)
            {
                std::string sId;
                if (is >> sId) revealedBranchSeriesIds.push_back(sId);
            }

            // Node states
            size_t stateCount = 0;
            if (!(is >> stateCount)) return false;
            for (size_t i = 0; i < stateCount; ++i)
            {
                std::string nId;
                uint32_t sVal = 0;
                if (is >> nId >> sVal) nodeStates[nId] = static_cast<uint8_t>(sVal);
            }

            // SubSystems
            if (!(is >> subSystems.levelBonus >> subSystems.refinementLevel >> subSystems.masteryXp >> subSystems.masteryMultiplier))
            {
                return false;
            }

            return true;
        }
    };
}
