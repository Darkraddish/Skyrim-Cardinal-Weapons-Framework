ScriptName CWFramework_WeaponManagerAPI Hidden

bool Function InitiateWeaponSelection(string asWeaponTypeId) Global Native
bool Function ConfirmWeaponChoice(string asWeaponTypeId) Global Native
string Function GetPendingWeaponChoice() Global Native
Function CancelWeaponSelection() Global Native
bool Function SwapActiveForm(string asNewFormId) Global Native
bool Function OnWeaponEquipped(string asItemFormId) Global Native
Function OnWeaponUnequipped(string asItemFormId) Global Native
bool Function IsCardinalWeaponEquipped() Global Native
string Function GetEquippedCardinalWeaponItemFormId() Global Native
