ScriptName CWFramework_AutoStartScript extends Quest

Book Property CW_CardinalLegendBook Auto
Quest Property CW_AwakeningQuest Auto

ObjectReference Property PlacedCupboardRef Auto Hidden

event OnInit()
    Debug.Trace("[CWFramework] CWFramework_AutoStartScript Quest OnInit fired!")
    RegisterForSingleUpdate(1.0)
    RegisterForKey(24) ; 'O' key
    AutoStart()
    PlaceBookInFarengarCupboard()
endEvent

event OnPlayerLoadGame()
    Debug.Trace("[CWFramework] CWFramework_AutoStartScript Quest OnPlayerLoadGame fired!")
    RegisterForSingleUpdate(1.0)
    RegisterForKey(24) ; 'O' key
    AutoStart()
    PlaceBookInFarengarCupboard()
endEvent

Function PlaceBookInFarengarCupboard()
    if CWFramework_SaveAPI.IsWeaponLocked()
        return
    endif

    if PlacedCupboardRef == None
        ObjectReference cupboard = Game.GetForm(0x0006B709) as ObjectReference
        if cupboard == None
            cupboard = Game.GetForm(0x0006B70A) as ObjectReference
        endif
        if cupboard == None
            cupboard = Game.GetForm(0x0006B70B) as ObjectReference
        endif
        
        if cupboard && CW_CardinalLegendBook
            cupboard.AddItem(CW_CardinalLegendBook, 1, false)
            PlacedCupboardRef = cupboard
            Debug.Trace("[CWFramework] Successfully placed 'The Legend of the Four Cardinal Weapons' inside Farengar's Cupboard in Dragonsreach!")
        endif
    endif
EndFunction

Function AutoStart()
    if CW_AwakeningQuest
        if !CW_AwakeningQuest.IsRunning()
            CW_AwakeningQuest.Start()
            Debug.Trace("[CWFramework] Started CW_AwakeningQuest.")
        endif
    endif
EndFunction

event OnKeyDown(int keyCode)
    if keyCode == 24 ; 'O' key
        Debug.Trace("[CWFramework] Key 'O' pressed in Quest AutoStartScript.")
        if CW_AwakeningQuest
            (CW_AwakeningQuest as CWFramework_TalosAwakeningQuest).OpenCardinalWeaponMenu()
        endif
    endif
endEvent

event OnUpdate()
    RegisterForKey(24) ; 'O' key
    RegisterForSingleUpdate(1.0)
    AutoStart()
    PlaceBookInFarengarCupboard()
endEvent
