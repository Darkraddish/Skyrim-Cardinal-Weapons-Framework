#include "DataLoader.h"
#include <fstream>
#include <filesystem>
#include <nlohmann/json.hpp>
#include <RE/Skyrim.h>
#include <SKSE/SKSE.h>

namespace fs = std::filesystem;
using json = nlohmann::json;

namespace CWF {
    DataLoader& DataLoader::GetSingleton() {
        static DataLoader instance;
        return instance;
    }

    void DataLoader::LoadAllContentPacks() {
        SKSE::log::info("[CWF] DataLoader scanning Data/ContentPacks/...");
        fs::path searchPath = "Data/ContentPacks";
        
        if (!fs::exists(searchPath)) {
            SKSE::log::info("[CWF] No ContentPacks directory found.");
            return;
        }

        for (const auto& entry : fs::recursive_directory_iterator(searchPath)) {
            if (entry.is_regular_file() && entry.path().extension() == ".json") {
                SKSE::log::info("[CWF] Loading JSON content: {}", entry.path().string());
                try {
                    std::ifstream f(entry.path());
                    json data = json::parse(f);
                    // Parse node, requirement, or ability entries based on structure
                } catch (const std::exception& e) {
                    SKSE::log::error("[CWF] Graceful validation: failed to parse {}: {}", entry.path().string(), e.what());
                }
            }
        }
    }

    std::string DataLoader::GetNodeState(const std::string& nodeId) const {
        auto it = m_nodes.find(nodeId);
        if (it != m_nodes.end()) {
            return it->second.state;
        }
        return "locked";
    }

    int DataLoader::GetRequirementProgress(const std::string& reqId) const {
        return 0;
    }
}
