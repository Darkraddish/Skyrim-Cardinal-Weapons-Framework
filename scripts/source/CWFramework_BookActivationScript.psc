ScriptName CWFramework_BookActivationScript extends ObjectReference

Quest Property CW_AwakeningQuest Auto

event OnRead()
    TriggerAwakening()
endEvent

event OnActivate(ObjectReference akActionRef)
    if akActionRef == Game.GetPlayer()
        TriggerAwakening()
    endif
endEvent

Function TriggerAwakening()
    if CWFramework_SaveAPI.IsWeaponLocked()
        return
    endif

    if CW_AwakeningQuest
        if !CW_AwakeningQuest.IsRunning()
            CW_AwakeningQuest.Start()
        endif
        (CW_AwakeningQuest as CWFramework_TalosAwakeningQuest).OnBookRead()
    endif
EndFunction
