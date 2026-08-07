ScriptName CWF_MCMScript Extends SKI_ConfigBase
; ==============================================================================
; Cardinal Weapon Framework — MCM Script
; Purpose: SkyUI MCM configuration menu handling hotkey binding for CWF_MenuHotkey.
; FormIDs referenced: CWF_MenuHotkey (GlobalVariable)
; ==============================================================================

GlobalVariable Property CWF_MenuHotkey Auto

Int OID_MenuHotkey

Event OnConfigInit()
    ModName = "Cardinal Weapon Framework"
EndEvent

Event OnPageReset(String page)
    SetCursorFillMode(TOP_TO_BOTTOM)
    AddHeaderOption("General Settings")
    OID_MenuHotkey = AddKeyMapOption("Menu Hotkey", CWF_MenuHotkey.GetValue() As Int)
EndEvent

Event OnOptionKeyMapChange(Int option, Int keycode, String conflictControl, String conflictName)
    If option == OID_MenuHotkey
        CWF_MenuHotkey.SetValue(keycode As Float)
        SetKeyMapOptionValue(OID_MenuHotkey, keycode)
    EndIf
EndEvent

Event OnOptionHighlight(Int option)
    If option == OID_MenuHotkey
        SetInfoText("Press this key to open the Legendary Weapon Progression Web.")
    EndIf
EndEvent
