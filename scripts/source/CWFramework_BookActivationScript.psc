ScriptName CWFramework_BookActivationScript extends ObjectReference

Quest Property CW_AwakeningQuest Auto

event OnRead()
    Debug.Trace("[CWFramework] Book OnRead event fired! Registering 0.2s update for Selection Menu.")
    RegisterForSingleUpdate(0.2)
endEvent

event OnActivate(ObjectReference akActionRef)
    if akActionRef == Game.GetPlayer()
        Debug.Trace("[CWFramework] Book OnActivate event fired! Registering 0.2s update for Selection Menu.")
        RegisterForSingleUpdate(0.2)
    endif
endEvent

event OnUpdate()
    TriggerAwakeningMenu()
endEvent

Function TriggerAwakeningMenu()
    if CW_AwakeningQuest
        if !CW_AwakeningQuest.IsRunning()
            CW_AwakeningQuest.Start()
        endif
        (CW_AwakeningQuest as CWFramework_TalosAwakeningQuest).ShowSelectionMenu()
    endif
EndFunction
