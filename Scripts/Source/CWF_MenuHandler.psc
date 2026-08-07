ScriptName CWF_MenuHandler Extends Quest
; ==============================================================================
; Cardinal Weapon Framework — Menu Handler
; Purpose: Papyrus bridge for opening the custom Scaleform UI menu
;          and exchanging node graph data with ActionScript.
; FormIDs referenced: CWF_HasSelectedWeapon (GlobalVariable)
; Talks to: SKSE InputHandler, CWF_ProgressionManager, Scaleform CWFramework.swf
; ==============================================================================

GlobalVariable Property CWF_HasSelectedWeapon Auto

Function OpenConstellationMenu()
    ; Open menu ONLY if weapon selection has been completed (or show selection screen if 0)
    If CWF_HasSelectedWeapon.GetValue() == 0.0
        Debug.Notification("You must read the tome and choose a Legendary Weapon first.")
        Return
    EndIf

    Debug.Trace("[CWF] Opening Scaleform Constellation Menu...")
    ; UI.OpenCustomMenu("CWFramework")
EndFunction

Function PushDataToMenu()
    ; Pushes node states, requirements, and abilities to Scaleform data provider
EndFunction

Event OnMenuClosed(String asMenuName)
    ; Clean up UI state
EndEvent

; TODO(Phase 7 & Phase 8): Complete Scaleform custom menu registration & data serialization
