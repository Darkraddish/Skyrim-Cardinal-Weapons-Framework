ScriptName CWFramework_ShrineTalosOverrideScript extends ObjectReference

event OnActivate(ObjectReference akActionRef)
    if akActionRef == Game.GetPlayer()
        CWFramework_TalosAwakeningQuest q = Game.GetFormFromFile(0x00000802, "CardinalWeaponsFramework.esp") as CWFramework_TalosAwakeningQuest
        if q
            if !q.IsRunning()
                q.Start()
            endif
            q.OnTalosShrineWorshipped()
        endif
    endif
endEvent
