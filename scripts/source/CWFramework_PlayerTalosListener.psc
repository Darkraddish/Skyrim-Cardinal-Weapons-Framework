ScriptName CWFramework_PlayerTalosListener extends ReferenceAlias

Book Property CW_CardinalLegendBook Auto
Quest Property CW_AwakeningQuest Auto

event OnInit()
    Debug.Trace("[CWFramework] CWFramework_PlayerTalosListener Initialized on PlayerRef Alias.")
    RegisterForSingleUpdate(1.0)
    RegisterForKey(24) ; 'O' key
    ForceInitQuests()
    CheckForTalosBlessing()
endEvent

event OnPlayerLoadGame()
    Debug.Trace("[CWFramework] CWFramework_PlayerTalosListener OnPlayerLoadGame event fired.")
    RegisterForSingleUpdate(1.0)
    RegisterForKey(24) ; 'O' key
    ForceInitQuests()
    CheckForTalosBlessing()
endEvent

Function ForceInitQuests()
    if CW_AwakeningQuest
        if !CW_AwakeningQuest.IsRunning()
            CW_AwakeningQuest.Start()
            Debug.Trace("[CWFramework] Force-started CW_AwakeningQuest on save load.")
        endif
    endif
EndFunction

event OnSpellAdded(Spell akSpell)
    if akSpell
        Debug.Trace("[CWFramework] Spell Added to Player: " + akSpell.GetName())
        string sName = akSpell.GetName()
        if sName == "Blessing of Talos" || sName == "Shrine of Talos" || akSpell.HasKeywordString("BlessingTalos")
            GiveBookToPlayer()
        endif
    endif
endEvent

bool Function PlayerHasTalosBlessing(Actor p)
    if !p
        return false
    endif
    
    MagicEffect e1 = Game.GetForm(0x000FB992) as MagicEffect
    MagicEffect e2 = Game.GetForm(0x000E0CFC) as MagicEffect
    if (e1 && p.HasMagicEffect(e1)) || (e2 && p.HasMagicEffect(e2))
        return true
    endif

    Spell s1 = Game.GetForm(0x000FB988) as Spell
    Spell s2 = Game.GetForm(0x000E0CFB) as Spell
    Spell s3 = Game.GetForm(0x0010E869) as Spell
    if (s1 && p.HasSpell(s1)) || (s2 && p.HasSpell(s2)) || (s3 && p.HasSpell(s3))
        return true
    endif

    int i = p.GetSpellCount()
    while i > 0
        i -= 1
        Spell sp = p.GetNthSpell(i)
        if sp
            string n = sp.GetName()
            if n == "Blessing of Talos" || n == "Shrine of Talos" || sp.HasKeywordString("BlessingTalos") || sp.HasKeywordString("ShrineTalos")
                return true
            endif
        endif
    endWhile

    return false
EndFunction

Function CheckForTalosBlessing()
    Actor p = Game.GetPlayer()
    if p && !CWFramework_SaveAPI.IsWeaponLocked()
        if PlayerHasTalosBlessing(p)
            Debug.Trace("[CWFramework] Active Blessing of Talos detected on player!")
            GiveBookToPlayer()
        endif
    endif
EndFunction

Function GiveBookToPlayer()
    if CWFramework_SaveAPI.IsWeaponLocked()
        return
    endif
    
    Actor p = Game.GetPlayer()
    if p && CW_CardinalLegendBook
        if p.GetItemCount(CW_CardinalLegendBook) == 0
            p.AddItem(CW_CardinalLegendBook, 1, false)
            Debug.Trace("[CWFramework] Placed 'The Legend of the Four Cardinal Weapons' in player inventory.")
            Debug.Notification("You have worshipped the Shrine of Talos! 'The Legend of the Four Cardinal Weapons' has been placed in your inventory.")
        endif
    endif
EndFunction

event OnKeyDown(int keyCode)
    if keyCode == 24 ; 'O' key
        Debug.Trace("[CWFramework] Key 'O' pressed down.")
        if CWFramework_SaveAPI.IsWeaponLocked()
            Debug.Trace("[CWFramework] Weapon is locked. Opening Cardinal Weapon Menu...")
            if CW_AwakeningQuest
                (CW_AwakeningQuest as CWFramework_TalosAwakeningQuest).OpenCardinalWeaponMenu()
            endif
        else
            Actor p = Game.GetPlayer()
            if p
                CheckForTalosBlessing()
                if CW_CardinalLegendBook && p.GetItemCount(CW_CardinalLegendBook) > 0
                    Debug.Notification("Read 'The Legend of the Four Cardinal Weapons' in your inventory to awaken your weapon.")
                else
                    Debug.Notification("You must pray at a Shrine of Talos to receive 'The Legend of the Four Cardinal Weapons'.")
                endif
            endif
        endif
    endif
endEvent

event OnUpdate()
    RegisterForKey(24) ; 'O' key
    RegisterForSingleUpdate(1.0)
    CheckForTalosBlessing()
endEvent
