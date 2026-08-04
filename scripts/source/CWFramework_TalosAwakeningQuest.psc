ScriptName CWFramework_TalosAwakeningQuest extends Quest

Message Property CW_SelectionMenuMessage Auto
Form Property CW_StarterShieldItem Auto

bool bSelectionDone = false

event OnInit()
    Maintenance()
endEvent

Function Maintenance()
    if CWFramework_SaveAPI.IsWeaponLocked()
        bSelectionDone = true
        SetStage(100)
    else
        SetCurrentStageID(10)
        SetActive(true)
    endif
EndFunction

Function OnTalosShrineWorshipped()
    if bSelectionDone
        return
    endif

    if CWFramework_SaveAPI.IsWeaponLocked()
        bSelectionDone = true
        SetStage(100)
        CompleteQuest()
        return
    endif

    int choice = 0
    if CW_SelectionMenuMessage
        choice = CW_SelectionMenuMessage.Show(0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0)
    endif

    if choice == 0
        bool initOK = CWFramework_WeaponManagerAPI.InitiateWeaponSelection("CW_WT_SHIELD")
        bool confirmOK = CWFramework_WeaponManagerAPI.ConfirmWeaponChoice("CW_WT_SHIELD")
        
        bSelectionDone = true
        if CW_StarterShieldItem
            Game.GetPlayer().AddItem(CW_StarterShieldItem, 1, false)
        endif
        Debug.Notification("The Cardinal Shield has awakened! Your Shield Hero journey begins.")
        SetStage(100)
        CompleteQuest()
    endif
EndFunction
