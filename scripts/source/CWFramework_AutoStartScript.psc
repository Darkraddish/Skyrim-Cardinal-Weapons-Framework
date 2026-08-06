ScriptName CWFramework_AutoStartScript extends Quest

Book Property CW_CardinalLegendBook Auto
Quest Property CW_AwakeningQuest Auto

ObjectReference Property PlacedCupboardRef Auto Hidden

event OnInit()
    Debug.Trace("[CWFramework] AUTOSTART: Quest OnInit fired!")
    Debug.Notification("[CWFramework] AutoStart Quest initialized.")
    RegisterForSingleUpdate(1.0)
    RegisterForKey(24) ; 'O' key
    AutoStart()
    PlaceBookInFarengarCupboard()
endEvent

event OnPlayerLoadGame()
    Debug.Trace("[CWFramework] AUTOSTART: OnPlayerLoadGame fired!")
    Debug.Notification("[CWFramework] Player loaded game.")
    RegisterForSingleUpdate(1.0)
    RegisterForKey(24) ; 'O' key
    AutoStart()
    PlaceBookInFarengarCupboard()
endEvent

Function PlaceBookInFarengarCupboard()
    bool locked = CWFramework_SaveAPI.IsWeaponLocked()
    Debug.Trace("[CWFramework] AUTOSTART: PlaceBookInFarengarCupboard check. IsWeaponLocked=" + locked)
    if locked
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
            Debug.Trace("[CWFramework] AUTOSTART: Successfully placed 'The Legend of the Four Cardinal Weapons' inside Farengar's Cupboard in Dragonsreach!")
            Debug.Notification("[CWFramework] Tome placed inside Farengar's Cupboard in Dragonsreach.")
        else
            Debug.Trace("[CWFramework] AUTOSTART: Cupboard or Book property is None! Cupboard=" + cupboard + " Book=" + CW_CardinalLegendBook)
        endif
    endif
EndFunction

Function AutoStart()
    Debug.Trace("[CWFramework] AUTOSTART: AutoStart checking quest...")
    if CW_AwakeningQuest
        if !CW_AwakeningQuest.IsRunning()
            CW_AwakeningQuest.Start()
            Debug.Trace("[CWFramework] AUTOSTART: Started CW_AwakeningQuest successfully.")
        else
            Debug.Trace("[CWFramework] AUTOSTART: CW_AwakeningQuest is already running.")
        endif
    else
        Debug.Trace("[CWFramework] AUTOSTART: ERROR - CW_AwakeningQuest property is None!")
    endif
EndFunction

event OnKeyDown(int keyCode)
    if keyCode == 24 ; 'O' key
        Debug.Trace("[CWFramework] AUTOSTART: Key 'O' pressed in Papyrus!")
        Debug.Notification("[CWFramework] 'O' Key Pressed in Papyrus.")
        if CW_AwakeningQuest
            (CW_AwakeningQuest as CWFramework_TalosAwakeningQuest).OpenCardinalWeaponMenu()
        else
            Debug.Trace("[CWFramework] AUTOSTART: OnKeyDown ERROR - CW_AwakeningQuest property is None!")
        endif
    endif
endEvent

event OnUpdate()
    Debug.Trace("[CWFramework] AUTOSTART: OnUpdate tick.")
    RegisterForKey(24) ; 'O' key
    RegisterForSingleUpdate(1.0)
    AutoStart()
    PlaceBookInFarengarCupboard()
endEvent
