#pragma once

#include <string>
#include <vector>
#include <unordered_map>

namespace CWF {
    struct NodeData {
        std::string nodeId;
        std::string weaponType;
        std::string series;
        std::string formId;
        std::string parentNodeId;
        bool convertible{ false };
        std::vector<std::string> requirementIds;
        std::vector<std::string> abilityIds;
        std::vector<std::string> effectIds;
        std::string state{ "locked" };
    };

    struct RequirementData {
        std::string requirementId;
        std::string type;
        std::string target;
        int amountRequired{ 0 };
    };

    struct AbilityData {
        std::string abilityId;
        std::string category;
        std::string scope;
        std::string magicEffectFormId;
    };

    class DataLoader {
    public:
        static DataLoader& GetSingleton();
        
        void LoadAllContentPacks();
        std::string GetNodeState(const std::string& nodeId) const;
        int GetRequirementProgress(const std::string& reqId) const;

    private:
        DataLoader() = default;
        std::unordered_map<std::string, NodeData> m_nodes;
        std::unordered_map<std::string, RequirementData> m_requirements;
        std::unordered_map<std::string, AbilityData> m_abilities;
    };
}
