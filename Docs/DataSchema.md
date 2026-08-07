# Data Schema Documentation

The framework only knows six abstractions: **Weapon Type**, **Series**, **Form**, **Node**, **Requirement**, and **Ability**.

```
Weapon Type
 └─ Series
     └─ Weapon Form
         └─ Progression Node
             ├─ Requirements
             ├─ Abilities
             ├─ Recipes
             └─ Effects
```

## Node Schema (`CWF_Nodes.json`)
```json
{
  "nodeId": "string",
  "weaponType": "string",
  "series": "string",
  "formId": "string",
  "parentNodeId": "string | null",
  "convertible": "boolean",
  "requirementIds": ["array of requirementId strings"],
  "abilityIds": ["array of abilityId strings"],
  "effectIds": ["array of effectId strings"],
  "state": "locked | unlockable | unlocked"
}
```

## Requirement Schema (`CWF_Requirements.json`)
```json
{
  "requirementId": "string",
  "type": "KillCount | ItemAbsorb | WeaponCopy | Quest | Level | PrevNode | Crafting | Location | EventTrigger | Custom",
  "target": "string",
  "amountRequired": "number"
}
```

## Ability Schema (`CWF_Abilities.json`)
```json
{
  "abilityId": "string",
  "category": "Equip | Passive | Active | Master",
  "scope": "FormBound | Permanent",
  "magicEffectFormId": "string (0x000801XX format)"
}
```
