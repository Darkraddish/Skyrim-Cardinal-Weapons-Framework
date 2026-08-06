ScriptName CWFramework_TalosAwakeningQuest extends Quest

Message Property CW_SelectionMenuMessage Auto
Message Property CW_ShieldMenuMessage Auto
Form Property CW_StarterShieldItem Auto

bool bSelectionDone = false

event OnInit()
    Debug.Trace("[CWFramework] TALOS_QUEST: Quest OnInit fired!")
    Maintenance()
endEvent

Function Maintenance()
    bool locked = CWFramework_SaveAPI.IsWeaponLocked()
    Debug.Trace("[CWFramework] TALOS_QUEST: Maintenance check. IsWeaponLocked=" + locked + " bSelectionDone=" + bSelectionDone)
    if locked
        bSelectionDone = true
        SetStage(100)
    else
        SetCurrentStageID(10)
        SetActive(true)
    endif
EndFunction

Function ShowSelectionMenu()
    bool locked = CWFramework_SaveAPI.IsWeaponLocked()
    Debug.Trace("[CWFramework] TALOS_QUEST: ShowSelectionMenu called! IsWeaponLocked=" + locked + " bSelectionDone=" + bSelectionDone)
    Debug.Notification("[CWFramework] ShowSelectionMenu Called! Locked=" + locked)

    if locked || bSelectionDone
        Debug.Trace("[CWFramework] TALOS_QUEST: Weapon already locked or selection done. Exiting ShowSelectionMenu.")
        return
    endif

    Debug.Trace("[CWFramework] TALOS_QUEST: Displaying 4-Weapon Selection Menu... MessageProp=" + CW_SelectionMenuMessage)

    int choice = 4
    if CW_SelectionMenuMessage
        choice = CW_SelectionMenuMessage.Show(0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0)
        Debug.Trace("[CWFramework] TALOS_QUEST: SelectionMenuMessage returned choice: " + choice)
    else
        Debug.Trace("[CWFramework] TALOS_QUEST: ERROR - CW_SelectionMenuMessage property is None!")
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
        Debug.Trace("[CWFramework] TALOS_QUEST: Player deferred weapon selection. Choice was: " + choice)
        return
    endif

    bool initOK = CWFramework_WeaponManagerAPI.InitiateWeaponSelection(chosenType)
    bool confirmOK = CWFramework_WeaponManagerAPI.ConfirmWeaponChoice(chosenType)
    Debug.Trace("[CWFramework] TALOS_QUEST: C++ API Calls: InitiateOK=" + initOK + " ConfirmOK=" + confirmOK)
    
    bSelectionDone = true
    Actor p = Game.GetPlayer()
    
    if chosenType == "CW_WT_SHIELD" && CW_StarterShieldItem && p
        p.AddItem(CW_StarterShieldItem, 1, false)
        p.EquipItem(CW_StarterShieldItem, false, false)
        Debug.Trace("[CWFramework] TALOS_QUEST: Added and equipped CW_StarterShieldItem.")
    endif

    Debug.Notification("You have chosen the " + chosenName + "! Your Cardinal Weapon has awakened.")
    Debug.Notification("Press 'O' at any time to open your Cardinal Weapon Menu.")
    Debug.Trace("[CWFramework] TALOS_QUEST: Weapon choice confirmed successfully: " + chosenType)
    SetStage(100)
    CompleteQuest()
EndFunction

Function OpenCardinalWeaponMenu()
    bool locked = CWFramework_SaveAPI.IsWeaponLocked()
    Debug.Trace("[CWFramework] TALOS_QUEST: OpenCardinalWeaponMenu called. IsWeaponLocked=" + locked)
    Debug.Notification("[CWFramework] OpenCardinalWeaponMenu. Locked=" + locked)

    if !locked
        Debug.Trace("[CWFramework] TALOS_QUEST: Weapon not locked yet. Redirecting to ShowSelectionMenu().")
        ShowSelectionMenu()
        return
    endif

    Actor p = Game.GetPlayer()
    int choice = 0
    if CW_ShieldMenuMessage
        choice = CW_ShieldMenuMessage.Show(0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0)
        Debug.Trace("[CWFramework] TALOS_QUEST: ShieldMenuMessage returned choice: " + choice)
    else
        Debug.Trace("[CWFramework] TALOS_QUEST: ERROR - CW_ShieldMenuMessage property is None!")
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
