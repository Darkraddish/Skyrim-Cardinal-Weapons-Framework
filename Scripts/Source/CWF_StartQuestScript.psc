ScriptName CWF_StartQuestScript Extends Quest
; ==============================================================================
; Cardinal Weapon Framework — Start Quest Script
; Purpose: Controls the introductory quest logic (`CWF_StartQuest`).
;          Verifies starting book placement alias and triggers initial setup.
; FormIDs referenced: CWF_StartQuest (Quest), CWF_HasSelectedWeapon (GlobalVariable)
; Talks to: CWF_BookRefScript, CWF_LegendaryWeaponManager
; ==============================================================================

GlobalVariable Property CWF_HasSelectedWeapon Auto
ReferenceAlias Property StartingBookAlias Auto

Event OnInit()
    RegisterForSingleUpdate(2.0)
EndEvent

Event OnUpdate()
    If CWF_HasSelectedWeapon.GetValue() == 0.0
        If StartingBookAlias.GetReference() == None
            Debug.Trace("[CWF] Warning: CWF_StartingBook alias reference is empty!")
        EndIf
    EndIf
EndEvent

; TODO(Phase 5): Handle quest stage transitions upon book reading
