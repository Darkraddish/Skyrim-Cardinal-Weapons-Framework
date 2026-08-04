ScriptName CWFramework_UIAPI Hidden

; Opens standalone radial constellation perk menu overlay.
Function OpenConstellationMenu() Global Native

; Closes standalone radial constellation menu.
Function CloseConstellationMenu() Global Native

; Returns true if the radial UI menu is open.
bool Function IsConstellationMenuOpen() Global Native

; Unlocks a selected node from UI interaction layer.
bool Function SelectAndUnlockNode(string asNodeId) Global Native

; UI Dashboard Queries
string Function GetChosenWeaponTypeName() Global Native
string Function GetActiveSeriesName() Global Native
string Function GetCurrentFormName() Global Native
float Function GetEffectiveDamage() Global Native
float Function GetEffectiveArmor() Global Native
