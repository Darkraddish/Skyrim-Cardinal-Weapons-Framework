ScriptName CWFramework_BookActivationScript extends ObjectReference

Quest Property CW_AwakeningQuest Auto

event OnRead()
    Debug.Trace("[CWFramework] BOOK_SCRIPT: OnRead event fired! Registering 0.2s update for Selection Menu.")
    Debug.Notification("[CWFramework] Book Read Event Fired!")
    RegisterForSingleUpdate(0.2)
endEvent

event OnActivate(ObjectReference akActionRef)
    if akActionRef == Game.GetPlayer()
        Debug.Trace("[CWFramework] BOOK_SCRIPT: OnActivate event fired by player! Registering 0.2s update for Selection Menu.")
        Debug.Notification("[CWFramework] Book Activated by Player!")
        RegisterForSingleUpdate(0.2)
    endif
endEvent

event OnUpdate()
    Debug.Trace("[CWFramework] BOOK_SCRIPT: OnUpdate tick fired! Triggering awakening menu.")
    TriggerAwakeningMenu()
endEvent

Function TriggerAwakeningMenu()
    Debug.Trace("[CWFramework] BOOK_SCRIPT: TriggerAwakeningMenu called! CW_AwakeningQuest=" + CW_AwakeningQuest)
    if CW_AwakeningQuest
        if !CW_AwakeningQuest.IsRunning()
            CW_AwakeningQuest.Start()
            Debug.Trace("[CWFramework] BOOK_SCRIPT: Started CW_AwakeningQuest from Book script.")
        endif
        (CW_AwakeningQuest as CWFramework_TalosAwakeningQuest).ShowSelectionMenu()
    else
        Debug.Trace("[CWFramework] BOOK_SCRIPT: ERROR - CW_AwakeningQuest property is None on Book script!")
    endif
EndFunction
