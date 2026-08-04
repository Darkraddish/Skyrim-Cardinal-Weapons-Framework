ScriptName CWFramework_ProgressionAPI Hidden

; Unlocks a progression node by ID, evolving active form if applicable.
bool Function UnlockProgressionNode(string asNodeId) Global Native

; Resolves a convertible node to a new outcome form ID in-place.
bool Function ResolveConvertibleNode(string asNodeId, string asOutcomeFormId) Global Native

; Sub-System Operations
void Function AddFlatLevelBonus(float afBonus) Global Native
float Function GetLevelBonus() Global Native

bool Function UpgradeRefinementTier() Global Native
int Function GetRefinementLevel() Global Native
float Function GetRefinementMultiplier() Global Native

void Function AddMasteryXp(float afXpAmount) Global Native
float Function GetMasteryXp() Global Native
float Function GetMasteryMultiplier() Global Native

; Combined Damage and Armor Stat Calculations
float Function CalculateEffectiveDamage(float afBaseDamage) Global Native
float Function CalculateEffectiveArmor(float afBaseArmor) Global Native
