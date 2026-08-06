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

Function OnBookRead()
    if CWFramework_SaveAPI.IsWeaponLocked() || bSelectionDone
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
        Actor p = Game.GetPlayer()
        if CW_StarterShieldItem && p
            p.AddItem(CW_StarterShieldItem, 1, false)
            p.EquipItem(CW_StarterShieldItem, false, false)
        endif
        Debug.Notification("Your Cardinal Weapon has awakened! Press 'P' to open the Cardinal Weapon Menu.")
        SetStage(100)
        CompleteQuest()
    endif
EndFunction

Function OpenCardinalWeaponMenu()
    if !CWFramework_SaveAPI.IsWeaponLocked()
        Debug.Notification("You must pray at a Shrine of Talos to receive 'The Legend of the Four Cardinal Weapons'.")
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
