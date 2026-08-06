ScriptName CWFramework_AutoStartScript extends Quest

Book Property CW_CardinalLegendBook Auto
Quest Property CW_AwakeningQuest Auto

ObjectReference Property PlacedBookRef Auto Hidden

event OnInit()
    Debug.Trace("[CWFramework] CWFramework_AutoStartScript Quest OnInit fired!")
    RegisterForSingleUpdate(1.0)
    RegisterForKey(24) ; 'O' key
    AutoStart()
    PlaceBookInDragonsreach()
endEvent

event OnPlayerLoadGame()
    Debug.Trace("[CWFramework] CWFramework_AutoStartScript Quest OnPlayerLoadGame fired!")
    RegisterForSingleUpdate(1.0)
    RegisterForKey(24) ; 'O' key
    AutoStart()
    PlaceBookInDragonsreach()
endEvent

Function PlaceBookInDragonsreach()
    if CWFramework_SaveAPI.IsWeaponLocked()
        return
    endif

    if PlacedBookRef == None
        ; Cell 0x000165A7 is Whiterun Dragonsreach interior
        ObjectReference dragonsreachRef = Game.GetForm(0x000165A7) as ObjectReference
        Actor p = Game.GetPlayer()
        if p && CW_CardinalLegendBook
            ; Spawn book right on top of Farengar's alchemy table in Dragonsreach
            PlacedBookRef = p.PlaceAtMe(CW_CardinalLegendBook, 1, false, true)
            if PlacedBookRef
                PlacedBookRef.SetPosition(-760.0, -1150.0, -90.0)
                PlacedBookRef.SetAngle(0.0, 0.0, 45.0)
                PlacedBookRef.Enable(false)
                Debug.Trace("[CWFramework] Successfully placed 'The Legend of the Four Cardinal Weapons' on Farengar's Alchemy Table in Dragonsreach!")
            endif
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
    
    Actor p = Game.GetPlayer()
    if p && !CWFramework_SaveAPI.IsWeaponLocked()
        if CW_CardinalLegendBook && p.GetItemCount(CW_CardinalLegendBook) == 0
            p.AddItem(CW_CardinalLegendBook, 1, false)
            Debug.Trace("[CWFramework] Placed 'The Legend of the Four Cardinal Weapons' in player inventory on game load.")
            Debug.Notification("The Shrine of Talos has granted you 'The Legend of the Four Cardinal Weapons'!")
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
                if CW_CardinalLegendBook && p.GetItemCount(CW_CardinalLegendBook) == 0
                    p.AddItem(CW_CardinalLegendBook, 1, false)
                    Debug.Notification("The Shrine of Talos has granted you 'The Legend of the Four Cardinal Weapons'!")
                endif
                if CW_CardinalLegendBook && p.GetItemCount(CW_CardinalLegendBook) > 0
                    Debug.Notification("Read 'The Legend of the Four Cardinal Weapons' in your inventory to awaken your weapon.")
                endif
            endif
        endif
    endif
endEvent

event OnUpdate()
    RegisterForKey(24) ; 'O' key
    RegisterForSingleUpdate(1.0)
    AutoStart()
    PlaceBookInDragonsreach()
endEvent
