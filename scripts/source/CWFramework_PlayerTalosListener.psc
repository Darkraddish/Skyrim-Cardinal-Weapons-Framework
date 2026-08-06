ScriptName CWFramework_PlayerTalosListener extends ReferenceAlias

Book Property CW_CardinalLegendBook Auto
Quest Property CW_AwakeningQuest Auto

event OnInit()
    Debug.Trace("[CWFramework] TALOS_LISTENER: Player Alias Script OnInit fired!")
    RegisterForPlayerSleep()
endEvent

event OnPlayerLoadGame()
    Debug.Trace("[CWFramework] TALOS_LISTENER: Player Alias Script OnPlayerLoadGame fired!")
endEvent

event OnItemAdded(Form akBaseItem, int aiItemCount, ObjectReference akItemReference, ObjectReference akSourceContainer)
    Debug.Trace("[CWFramework] TALOS_LISTENER: OnItemAdded fired! Form=" + akBaseItem + " Count=" + aiItemCount)
    if akBaseItem == CW_CardinalLegendBook
        Debug.Trace("[CWFramework] TALOS_LISTENER: Player received 'The Legend of the Four Cardinal Weapons'!")
        Debug.Notification("[CWFramework] Obtained 'The Legend of the Four Cardinal Weapons'!")
    endif
endEvent
