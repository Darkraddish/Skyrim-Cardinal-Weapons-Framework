ScriptName CWFramework_AutoStartScript extends Quest

Book Property CW_CardinalLegendBook Auto
Quest Property CW_AwakeningQuest Auto

event OnInit()
    Debug.Trace("[CWFramework] CWFramework_AutoStartScript Quest OnInit fired!")
    RegisterForSingleUpdate(1.0)
    RegisterForKey(24) ; 'O' key
    AutoStart()
endEvent

event OnPlayerLoadGame()
    Debug.Trace("[CWFramework] CWFramework_AutoStartScript Quest OnPlayerLoadGame fired!")
    RegisterForSingleUpdate(1.0)
    RegisterForKey(24) ; 'O' key
    AutoStart()
endEvent

Function AutoStart()
    if CW_AwakeningQuest
        if !CW_AwakeningQuest.IsRunning()
            CW_AwakeningQuest.Start()
            Debug.Trace("[CWFramework] Started CW_AwakeningQuest.")
        endif
    endif
    CheckTalosBlessing()
EndFunction

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

Function CheckTalosBlessing()
    Actor p = Game.GetPlayer()
    if p && !CWFramework_SaveAPI.IsWeaponLocked()
        if PlayerHasTalosBlessing(p)
            if CW_CardinalLegendBook && p.GetItemCount(CW_CardinalLegendBook) == 0
                p.AddItem(CW_CardinalLegendBook, 1, false)
                Debug.Trace("[CWFramework] Granted 'The Legend of the Four Cardinal Weapons' to player inventory.")
                Debug.Notification("You have worshipped the Shrine of Talos! 'The Legend of the Four Cardinal Weapons' has been placed in your inventory.")
            endif
        endif
    endif
EndFunction

event OnKeyDown(int keyCode)
    if keyCode == 24 ; 'O' key
        Debug.Trace("[CWFramework] Key 'O' pressed in Quest AutoStartScript.")
        if CWFramework_SaveAPI.IsWeaponLocked()
            if CW_AwakeningQuest
                (CW_AwakeningQuest as CWFramework_TalosAwakeningQuest).OpenCardinalWeaponMenu()
            endif
        else
            Actor p = Game.GetPlayer()
            if p
                CheckTalosBlessing()
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
    CheckTalosBlessing()
endEvent
