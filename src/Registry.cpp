#include "CWFramework/Registry.h"
#include <mutex>
#include <fstream>
#include <sstream>
#include <iostream>
#include <filesystem>

namespace CWFramework
{
    namespace
    {
        std::string ExtractJsonField(const std::string& item, const std::string& key)
        {
            std::string searchKey = "\"" + key + "\":";
            auto pos = item.find(searchKey);
            if (pos == std::string::npos) return "";
            pos += searchKey.length();
            while (pos < item.length() && (item[pos] == ' ' || item[pos] == '\t' || item[pos] == '\r' || item[pos] == '\n')) pos++;
            if (pos < item.length() && item[pos] == '"')
            {
                pos++;
                auto endPos = item.find('"', pos);
                if (endPos != std::string::npos)
                {
                    return item.substr(pos, endPos - pos);
                }
            }
            return "";
        }

        std::vector<std::string> SplitJsonArrayObjects(const std::string& content)
        {
            std::vector<std::string> objects;
            bool inObj = false;
            int depth = 0;
            std::string current;
            for (char c : content)
            {
                if (c == '{')
                {
                    depth++;
                    inObj = true;
                }
                if (inObj) current += c;
                if (c == '}')
                {
                    depth--;
                    if (depth == 0)
                    {
                        objects.push_back(current);
                        current.clear();
                        inObj = false;
                    }
                }
            }
            return objects;
        }
    }

    bool Registry::LoadContentPacksFromDirectory(const std::string& directoryPath)
    {
        if (!std::filesystem::exists(directoryPath)) return false;

        // Load weapon_types.json
        std::string wtPath = directoryPath + "/weapon_types.json";
        std::ifstream wtFile(wtPath);
        if (wtFile.is_open())
        {
            std::stringstream buffer;
            buffer << wtFile.rdbuf();
            auto objects = SplitJsonArrayObjects(buffer.str());
            for (const auto& obj : objects)
            {
                WeaponTypeData wt;
                wt.id = ExtractJsonField(obj, "id");
                wt.name = ExtractJsonField(obj, "name");
                wt.description = ExtractJsonField(obj, "description");
                wt.iconAsset = ExtractJsonField(obj, "iconAsset");
                if (!wt.id.empty()) RegisterWeaponType(wt);
            }
        }

        // Load series.json
        std::string serPath = directoryPath + "/series.json";
        std::ifstream serFile(serPath);
        if (serFile.is_open())
        {
            std::stringstream buffer;
            buffer << serFile.rdbuf();
            auto objects = SplitJsonArrayObjects(buffer.str());
            for (const auto& obj : objects)
            {
                SeriesData s;
                s.id = ExtractJsonField(obj, "id");
                s.weaponTypeId = ExtractJsonField(obj, "weaponTypeId");
                s.name = ExtractJsonField(obj, "name");
                s.description = ExtractJsonField(obj, "description");
                s.iconAsset = ExtractJsonField(obj, "iconAsset");
                if (!s.id.empty()) RegisterSeries(s);
            }
        }

        // Load weapon_forms.json
        std::string formPath = directoryPath + "/weapon_forms.json";
        std::ifstream formFile(formPath);
        if (formFile.is_open())
        {
            std::stringstream buffer;
            buffer << formFile.rdbuf();
            auto objects = SplitJsonArrayObjects(buffer.str());
            for (const auto& obj : objects)
            {
                WeaponFormData f;
                f.id = ExtractJsonField(obj, "id");
                f.seriesId = ExtractJsonField(obj, "seriesId");
                f.name = ExtractJsonField(obj, "name");
                f.description = ExtractJsonField(obj, "description");
                f.modelAsset = ExtractJsonField(obj, "modelAsset");
                if (!f.id.empty()) RegisterWeaponForm(f);
            }
        }

        return true;
    }

    Registry::Registry()
    {
        LoadContentPacksFromDirectory("content_packs/cardinal_heroes");
        LoadContentPacksFromDirectory("Data/SKSE/Plugins/CWFramework/content_packs/cardinal_heroes");
    }

    bool Registry::RegisterWeaponType(const WeaponTypeData& data)
    {
        if (data.id.empty()) return false;
        std::unique_lock lock(m_mutex);
        m_weaponTypes[data.id] = data;
        return true;
    }

    bool Registry::RegisterSeries(const SeriesData& data)
    {
        if (data.id.empty()) return false;
        std::unique_lock lock(m_mutex);
        m_seriesMap[data.id] = data;
        return true;
    }

    bool Registry::RegisterWeaponForm(const WeaponFormData& data)
    {
        if (data.id.empty()) return false;
        std::unique_lock lock(m_mutex);
        m_weaponForms[data.id] = data;
        return true;
    }

    bool Registry::RegisterProgressionNode(const ProgressionNodeData& data)
    {
        if (data.id.empty()) return false;
        std::unique_lock lock(m_mutex);
        m_nodes[data.id] = data;
        return true;
    }

    bool Registry::RegisterRequirement(const RequirementData& data)
    {
        if (data.id.empty()) return false;
        std::unique_lock lock(m_mutex);
        m_requirements[data.id] = data;
        return true;
    }

    bool Registry::RegisterAbility(const AbilityData& data)
    {
        if (data.id.empty()) return false;
        std::unique_lock lock(m_mutex);
        m_abilities[data.id] = data;
        return true;
    }

    bool Registry::RegisterEffect(const EffectData& data)
    {
        if (data.id.empty()) return false;
        std::unique_lock lock(m_mutex);
        m_effects[data.id] = data;
        return true;
    }

    bool Registry::RegisterRecipe(const RecipeData& data)
    {
        if (data.id.empty()) return false;
        std::unique_lock lock(m_mutex);
        m_recipes[data.id] = data;
        return true;
    }

    std::optional<WeaponTypeData> Registry::GetWeaponType(const std::string& id) const
    {
        std::shared_lock lock(m_mutex);
        auto it = m_weaponTypes.find(id);
        if (it != m_weaponTypes.end()) return it->second;
        return std::nullopt;
    }

    std::optional<SeriesData> Registry::GetSeries(const std::string& id) const
    {
        std::shared_lock lock(m_mutex);
        auto it = m_seriesMap.find(id);
        if (it != m_seriesMap.end()) return it->second;
        return std::nullopt;
    }

    std::optional<WeaponFormData> Registry::GetWeaponForm(const std::string& id) const
    {
        std::shared_lock lock(m_mutex);
        auto it = m_weaponForms.find(id);
        if (it != m_weaponForms.end()) return it->second;
        return std::nullopt;
    }

    std::optional<ProgressionNodeData> Registry::GetProgressionNode(const std::string& id) const
    {
        std::shared_lock lock(m_mutex);
        auto it = m_nodes.find(id);
        if (it != m_nodes.end()) return it->second;
        return std::nullopt;
    }

    std::optional<RequirementData> Registry::GetRequirement(const std::string& id) const
    {
        std::shared_lock lock(m_mutex);
        auto it = m_requirements.find(id);
        if (it != m_requirements.end()) return it->second;
        return std::nullopt;
    }

    std::optional<AbilityData> Registry::GetAbility(const std::string& id) const
    {
        std::shared_lock lock(m_mutex);
        auto it = m_abilities.find(id);
        if (it != m_abilities.end()) return it->second;
        return std::nullopt;
    }

    std::optional<EffectData> Registry::GetEffect(const std::string& id) const
    {
        std::shared_lock lock(m_mutex);
        auto it = m_effects.find(id);
        if (it != m_effects.end()) return it->second;
        return std::nullopt;
    }

    std::optional<RecipeData> Registry::GetRecipe(const std::string& id) const
    {
        std::shared_lock lock(m_mutex);
        auto it = m_recipes.find(id);
        if (it != m_recipes.end()) return it->second;
        return std::nullopt;
    }

    std::vector<WeaponTypeData> Registry::GetAllWeaponTypes() const
    {
        std::shared_lock lock(m_mutex);
        std::vector<WeaponTypeData> result;
        result.reserve(m_weaponTypes.size());
        for (const auto& [id, data] : m_weaponTypes)
        {
            result.push_back(data);
        }
        return result;
    }

    std::vector<SeriesData> Registry::GetSeriesForWeaponType(const std::string& weaponTypeId) const
    {
        std::shared_lock lock(m_mutex);
        std::vector<SeriesData> result;
        for (const auto& [id, data] : m_seriesMap)
        {
            if (data.weaponTypeId == weaponTypeId)
            {
                result.push_back(data);
            }
        }
        return result;
    }

    std::vector<ProgressionNodeData> Registry::GetNodesForSeries(const std::string& seriesId) const
    {
        std::shared_lock lock(m_mutex);
        std::vector<ProgressionNodeData> result;
        for (const auto& [id, data] : m_nodes)
        {
            if (data.seriesId == seriesId)
            {
                result.push_back(data);
            }
        }
        return result;
    }

    size_t Registry::GetWeaponTypeCount() const
    {
        std::shared_lock lock(m_mutex);
        return m_weaponTypes.size();
    }

    size_t Registry::GetSeriesCount() const
    {
        std::shared_lock lock(m_mutex);
        return m_seriesMap.size();
    }

    size_t Registry::GetWeaponFormCount() const
    {
        std::shared_lock lock(m_mutex);
        return m_weaponForms.size();
    }

    size_t Registry::GetProgressionNodeCount() const
    {
        std::shared_lock lock(m_mutex);
        return m_nodes.size();
    }

    void Registry::Clear()
    {
        std::unique_lock lock(m_mutex);
        m_weaponTypes.clear();
        m_seriesMap.clear();
        m_weaponForms.clear();
        m_nodes.clear();
        m_requirements.clear();
        m_abilities.clear();
        m_effects.clear();
        m_recipes.clear();
    }
}
