ScriptName CWFramework_SpellActivationScript extends activemagiceffect

event OnEffectStart(Actor akTarget, Actor akCaster)
    CWFramework_TalosAwakeningQuest q = Game.GetFormFromFile(0x00000802, "CardinalWeaponsFramework.esp") as CWFramework_TalosAwakeningQuest
    if q
        if !q.IsRunning()
            q.Start()
        endif
        if CWFramework_SaveAPI.IsWeaponLocked()
            q.OpenShieldMenu()
        else
            q.OnTalosShrineWorshipped()
        endif
    endif
endEvent
