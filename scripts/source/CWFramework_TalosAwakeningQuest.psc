ScriptName CWFramework_TalosAwakeningQuest extends Quest

Message Property CW_SelectionMenuMessage Auto
Message Property CW_ShieldMenuMessage Auto
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

Function ShowSelectionMenu()
    if CWFramework_SaveAPI.IsWeaponLocked() || bSelectionDone
        return
    endif

    Debug.Trace("[CWFramework] Displaying 4-Weapon Selection Menu...")

    int choice = 4
    if CW_SelectionMenuMessage
        choice = CW_SelectionMenuMessage.Show(0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0)
    endif

    string chosenType = ""
    string chosenName = ""

    if choice == 0
        chosenType = "CW_WT_SHIELD"
        chosenName = "Cardinal Shield"
    elseif choice == 1
        chosenType = "CW_WT_SWORD"
        chosenName = "Cardinal Sword"
    elseif choice == 2
        chosenType = "CW_WT_SPEAR"
        chosenName = "Cardinal Spear"
    elseif choice == 3
        chosenType = "CW_WT_BOW"
        chosenName = "Cardinal Bow"
    else
        Debug.Trace("[CWFramework] Player deferred weapon selection.")
        return
    endif

    bool initOK = CWFramework_WeaponManagerAPI.InitiateWeaponSelection(chosenType)
    bool confirmOK = CWFramework_WeaponManagerAPI.ConfirmWeaponChoice(chosenType)
    
    bSelectionDone = true
    Actor p = Game.GetPlayer()
    
    if chosenType == "CW_WT_SHIELD" && CW_StarterShieldItem && p
        p.AddItem(CW_StarterShieldItem, 1, false)
        p.EquipItem(CW_StarterShieldItem, false, false)
    endif

    Debug.Notification("You have chosen the " + chosenName + "! Your Cardinal Weapon has awakened.")
    Debug.Notification("Press 'O' at any time to open your Cardinal Weapon Menu.")
    Debug.Trace("[CWFramework] Weapon choice confirmed: " + chosenType)
    SetStage(100)
    CompleteQuest()
EndFunction

Function OpenCardinalWeaponMenu()
    if !CWFramework_SaveAPI.IsWeaponLocked()
        ShowSelectionMenu()
        return
    endif

    Actor p = Game.GetPlayer()
    int choice = 0
    if CW_ShieldMenuMessage
        choice = CW_ShieldMenuMessage.Show(0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0)
    endif

    if choice == 0
        if CW_StarterShieldItem && p
            p.EquipItem(CW_StarterShieldItem, false, false)
            Debug.Notification("Equipped Cardinal Weapon.")
        endif
    elseif choice == 1
        float dmg = CWFramework_UIAPI.GetEffectiveDamage()
        float arm = CWFramework_UIAPI.GetEffectiveArmor()
        Debug.Notification("Cardinal Weapon Status: Form=" + CWFramework_UIAPI.GetCurrentFormName() + " | Armor=" + arm + " | Dmg=" + dmg)
    elseif choice == 2
        CWFramework_UIAPI.OpenConstellationMenu()
        Debug.Notification("Opening Progression Tree...")
    endif
EndFunction
