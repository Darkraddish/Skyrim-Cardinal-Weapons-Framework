ScriptName CWFramework_PlayerHotkeyListener extends ReferenceAlias

int Property TreeHotkey = 25 Auto ; 'P' key scancode 25
Quest Property CW_AwakeningQuest Auto
Book Property CW_CardinalLegendBook Auto

event OnInit()
    RegisterForSingleUpdate(1.0)
    RegisterForKey(TreeHotkey)
endEvent

event OnPlayerLoadGame()
    RegisterForSingleUpdate(1.0)
    RegisterForKey(TreeHotkey)
endEvent

event OnKeyDown(int keyCode)
    if keyCode == TreeHotkey
        if CWFramework_SaveAPI.IsWeaponLocked()
            if CW_AwakeningQuest
                (CW_AwakeningQuest as CWFramework_TalosAwakeningQuest).OpenCardinalWeaponMenu()
            endif
        else
            Actor p = Game.GetPlayer()
            if p && CW_CardinalLegendBook
                if p.GetItemCount(CW_CardinalLegendBook) == 0
                    p.AddItem(CW_CardinalLegendBook, 1, false)
                    Debug.Notification("The tome 'The Legend of the Four Cardinal Weapons' has been added to your inventory. Read it to awaken your weapon.")
                else
                    Debug.Notification("Read 'The Legend of the Four Cardinal Weapons' in your inventory to awaken your weapon.")
                endif
            else
                Debug.Notification("No Cardinal Weapon has been awakened yet.")
            endif
        endif
    endif
endEvent

event OnUpdate()
    RegisterForKey(TreeHotkey)
    RegisterForSingleUpdate(5.0)
endEvent
