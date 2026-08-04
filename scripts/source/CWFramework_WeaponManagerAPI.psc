ScriptName CWFramework_WeaponManagerAPI Hidden

; Initiates player weapon selection flow for a registered weapon type ID.
; Returns true if selection started successfully, false if invalid or choice already locked.
bool Function InitiateWeaponSelection(string asWeaponTypeId) Global Native

; Confirms weapon selection and permanently locks choice into character save data.
bool Function ConfirmWeaponChoice(string asWeaponTypeId) Global Native

; Returns the pending weapon choice ID, if selection is currently in progress.
string Function GetPendingWeaponChoice() Global Native

; Cancels pending weapon selection flow.
Function CancelWeaponSelection() Global Native

; Swaps current active weapon form to a new form ID.
bool Function SwapActiveForm(string asNewFormId) Global Native

; Event handler called when a player equips an item.
; Enforces single cardinal weapon equipping and handles form activation.
; Returns true if allowed, false if rejected (e.g. attempting to dual-wield multiple cardinal weapons).
bool Function OnWeaponEquipped(string asItemFormId) Global Native

; Event handler called when a player unequips an item.
Function OnWeaponUnequipped(string asItemFormId) Global Native

; Returns true if a registered Cardinal Weapon is currently equipped by the player.
bool Function IsCardinalWeaponEquipped() Global Native

; Returns the item form ID of the currently equipped Cardinal Weapon.
string Function GetEquippedCardinalWeaponItemFormId() Global Native
