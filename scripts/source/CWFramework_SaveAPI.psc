ScriptName CWFramework_SaveAPI Hidden

; Permanently locks in player's single Legendary Weapon choice for this save file.
; Returns true on successful lock, false if weapon already locked or invalid.
bool Function ChooseLegendaryWeapon(string asWeaponTypeId) Global Native

; Returns true if the player has locked in their single weapon choice for this save.
bool Function IsWeaponLocked() Global Native

; Returns the locked weapon type ID for this save (e.g., "CW_WT_SHIELD").
string Function GetChosenWeaponTypeId() Global Native

; Sets the active equipped weapon form ID.
bool Function SetCurrentForm(string asFormId) Global Native

; Returns the active weapon form ID.
string Function GetCurrentFormId() Global Native

; Unlocks a progression node by ID.
bool Function UnlockNode(string asNodeId) Global Native

; Returns true if a specific node is unlocked.
bool Function IsNodeUnlocked(string asNodeId) Global Native

; Returns node visual/availability state (0 = Locked, 1 = Unlockable, 2 = Unlocked).
int Function GetNodeState(string asNodeId) Global Native
