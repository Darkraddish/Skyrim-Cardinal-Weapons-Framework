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

Function CheckTalosBlessing()
    Actor p = Game.GetPlayer()
    if p && !CWFramework_SaveAPI.IsWeaponLocked()
        MagicEffect e1 = Game.GetForm(0x000FB992) as MagicEffect
        Spell s1 = Game.GetForm(0x000FB988) as Spell
        Spell s2 = Game.GetForm(0x000E0CFB) as Spell
        
        bool hasTalos = false
        if (e1 && p.HasMagicEffect(e1)) || (s1 && p.HasSpell(s1)) || (s2 && p.HasSpell(s2))
            hasTalos = true
        endif
        
        if hasTalos
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
    RegisterForSingleUpdate(2.0)
    CheckTalosBlessing()
endEvent
