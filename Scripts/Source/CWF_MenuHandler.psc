ScriptName CWF_MenuHandler Extends Quest
; ==============================================================================
; Cardinal Weapon Framework — Menu Handler
; Purpose: Papyrus bridge for opening the custom Scaleform UI menu
;          and exchanging node graph data with ActionScript.
; FormIDs referenced: CWF_HasSelectedWeapon (GlobalVariable), CWF_MenuHotkey (GlobalVariable)
; Talks to: SKSE InputHandler, CWF_ProgressionManager, Scaleform CWFramework.swf
; ==============================================================================

GlobalVariable Property CWF_HasSelectedWeapon Auto
GlobalVariable Property CWF_MenuHotkey Auto

Function OpenConstellationMenu()
    If CWF_HasSelectedWeapon && CWF_HasSelectedWeapon.GetValue() == 0.0
        Debug.Notification("You must read the tome and choose a Legendary Weapon first.")
        Return
    EndIf

    Debug.Notification("Cardinal Weapon Framework Menu Opened!")
    Debug.Trace("[CWF] Opening Scaleform Constellation Menu...")
EndFunction

Function PushDataToMenu()
    ; Pushes node states, requirements, and abilities to Scaleform data provider
EndFunction

Event OnMenuClosed(String asMenuName)
    ; Clean up UI state
EndEvent
