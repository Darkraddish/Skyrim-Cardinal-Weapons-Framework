ScriptName CWFramework_ShrineTalosScript extends ObjectReference

Book Property CW_CardinalLegendBook Auto

event OnActivate(ObjectReference akActionRef)
    Actor p = akActionRef as Actor
    if p == Game.GetPlayer()
        Debug.Trace("[CWFramework] Shrine of Talos OnActivate fired by Player.")
        if !CWFramework_SaveAPI.IsWeaponLocked()
            if CW_CardinalLegendBook && p.GetItemCount(CW_CardinalLegendBook) == 0
                p.AddItem(CW_CardinalLegendBook, 1, false)
                Debug.Trace("[CWFramework] Shrine of Talos granted 'The Legend of the Four Cardinal Weapons' to player inventory.")
                Debug.Notification("You have worshipped the Shrine of Talos! 'The Legend of the Four Cardinal Weapons' has been placed in your inventory.")
            endif
        endif
    endif
endEvent
