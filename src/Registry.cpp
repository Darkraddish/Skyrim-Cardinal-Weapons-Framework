#include "CWFramework/Registry.h"
#include <mutex>

namespace CWFramework
{
    Registry::Registry()
    {
        WeaponTypeData shieldType;
        shieldType.id = "CW_WT_SHIELD";
        shieldType.name = "Cardinal Shield";
        shieldType.description = "The immovable fortress of the legendary cardinal heroes.";
        m_weaponTypes[shieldType.id] = shieldType;

        WeaponTypeData swordType;
        swordType.id = "CW_WT_SWORD";
        swordType.name = "Cardinal Sword";
        swordType.description = "The unyielding blade of the legendary cardinal heroes.";
        m_weaponTypes[swordType.id] = swordType;

        WeaponTypeData spearType;
        spearType.id = "CW_WT_SPEAR";
        spearType.name = "Cardinal Spear";
        spearType.description = "The piercing lance of the legendary cardinal heroes.";
        m_weaponTypes[spearType.id] = spearType;

        WeaponTypeData bowType;
        bowType.id = "CW_WT_BOW";
        bowType.name = "Cardinal Bow";
        bowType.description = "The swift marksmanship of the legendary cardinal heroes.";
        m_weaponTypes[bowType.id] = bowType;
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
