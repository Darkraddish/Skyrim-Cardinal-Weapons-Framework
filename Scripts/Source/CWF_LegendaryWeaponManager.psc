ScriptName CWF_LegendaryWeaponManager Extends Quest
; ==============================================================================
; Cardinal Weapon Framework — Legendary Weapon Manager
; Purpose: Registers weapon types, tracks player's chosen legendary weapon, and
;          enforces "exactly one legendary weapon per save, fixed for lifetime".
; FormIDs referenced: CWF_HasSelectedWeapon (GlobalVariable)
; Talks to: CWF_ProgressionManager, CWF_ContentValidator, CWF_MenuHandler
; ==============================================================================

GlobalVariable Property CWF_HasSelectedWeapon Auto
String Property SelectedWeaponType Auto Hidden
String Property CurrentWeaponFormId Auto Hidden

Event OnInit()
    RegisterForSingleUpdate(1.0)
EndEvent

Event OnUpdate()
    ; Initialize framework state if not already selected
EndEvent

Function RegisterWeaponType(String asWeaponType)
    ; Registers a valid weapon type (e.g. Shield, Sword, Bow, Spear)
EndFunction

Bool Function SelectLegendaryWeapon(String asWeaponType)
    ; Rule: Once selected, fixed for the save's lifetime.
    If CWF_HasSelectedWeapon.GetValue() > 0.0
        Debug.Trace("[CWF] Cannot select legendary weapon: weapon already chosen for this save.")
        Return False
    EndIf

    SelectedWeaponType = asWeaponType
    CWF_HasSelectedWeapon.SetValue(1.0)
    Debug.Trace("[CWF] Legendary weapon selected: " + asWeaponType)
    Return True
EndFunction

String Function GetSelectedWeaponType()
    Return SelectedWeaponType
EndFunction

; TODO(Phase 3): Wire SKSE co-save persistence callbacks for weapon selection state
