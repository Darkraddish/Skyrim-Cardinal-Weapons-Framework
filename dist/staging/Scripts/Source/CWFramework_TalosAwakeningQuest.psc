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

    ; Display selection dialog
    int choice = 0
    if CW_SelectionMenuMessage
        choice = CW_SelectionMenuMessage.Show()
    endif

    if choice == 0
        ; Player chose Cardinal Shield
        bool initOK = CWFramework_WeaponManagerAPI.InitiateWeaponSelection("CW_WT_SHIELD")
        bool confirmOK = CWFramework_WeaponManagerAPI.ConfirmWeaponChoice("CW_WT_SHIELD")
        
        if confirmOK || initOK
            bSelectionDone = true
            if CW_StarterShieldItem
                Game.GetPlayer().AddItem(CW_StarterShieldItem, 1, false)
            endif
            Debug.Notification("The Cardinal Shield has awakened! Your Shield Hero journey begins.")
            SetStage(100)
            CompleteQuest()
        else
            Debug.Notification("Cardinal Shield choice confirmed!")
            bSelectionDone = true
            SetStage(100)
            CompleteQuest()
        endif
    endif
EndFunction
