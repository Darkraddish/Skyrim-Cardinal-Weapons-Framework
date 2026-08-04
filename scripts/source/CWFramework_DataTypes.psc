ScriptName CWFramework_DataTypes Hidden

; Node Types
int property NODE_TYPE_FORM = 0 AutoReadOnly
int property NODE_TYPE_PASSIVE_UPGRADE = 1 AutoReadOnly
int property NODE_TYPE_ABILITY_UNLOCK = 2 AutoReadOnly
int property NODE_TYPE_TRANSFORMATION = 3 AutoReadOnly
int property NODE_TYPE_REFINEMENT_MILESTONE = 4 AutoReadOnly
int property NODE_TYPE_EVENT_BRANCH = 5 AutoReadOnly

; Node Visual/State Markers
int property NODE_STATE_LOCKED = 0 AutoReadOnly
int property NODE_STATE_UNLOCKABLE = 1 AutoReadOnly
int property NODE_STATE_UNLOCKED = 2 AutoReadOnly

; Ability Scopes
int property ABILITY_SCOPE_EQUIP = 0 AutoReadOnly ; Form-bound: active while form equipped
int property ABILITY_SCOPE_ACTIVE = 1 AutoReadOnly; Form-bound: active skill while form equipped
int property ABILITY_SCOPE_PASSIVE = 2 AutoReadOnly; Permanent: active across all forms
int property ABILITY_SCOPE_MASTER = 3 AutoReadOnly ; Permanent: pinnacle skill across all forms

; Requirement Types
int property REQ_TYPE_KILL_COUNT = 0 AutoReadOnly
int property REQ_TYPE_ITEM_ABSORPTION = 1 AutoReadOnly
int property REQ_TYPE_WEAPON_COPY = 2 AutoReadOnly
int property REQ_TYPE_QUEST_STATE = 3 AutoReadOnly
int property REQ_TYPE_PLAYER_LEVEL = 4 AutoReadOnly
int property REQ_TYPE_PARENT_DEPENDENCY = 5 AutoReadOnly
int property REQ_TYPE_CRAFTING = 6 AutoReadOnly
int property REQ_TYPE_LOCATION = 7 AutoReadOnly
int property REQ_TYPE_SPECIAL_EVENT = 8 AutoReadOnly
int property REQ_TYPE_CUSTOM_SCRIPT = 9 AutoReadOnly

; Stat Modifier Types
int property MODIFIER_FLAT_ADDITIVE = 0 AutoReadOnly
int property MODIFIER_PERCENTAGE_MULTIPLIER = 1 AutoReadOnly
