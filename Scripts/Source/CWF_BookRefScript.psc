ScriptName CWF_BookRefScript Extends ReferenceAlias
; ==============================================================================
; Cardinal Weapon Framework — Book Reference Script
; Purpose: Attached to the CWF_StartingBook ObjectReference alias.
;          Triggers introductory quest completion upon reading.
; FormIDs referenced: CWF_StartQuest (Quest)
; Talks to: CWF_StartQuestScript, CWF_MenuHandler
; ==============================================================================

Quest Property CWF_StartQuest Auto

Event OnRead()
    Debug.Notification("You open the ancient tome and feel a surge of legendary energy...")
    Debug.Trace("[CWF] CWF_StartingBook read event triggered.")
    If CWF_StartQuest
        CWF_StartQuest.SetStage(100)
        CWF_StartQuest.CompleteQuest()
    EndIf
EndEvent

; Fallback event support if OnRead is unhandled by specific engine versions:
Event OnContainerChanged(ObjectReference akNewContainer, ObjectReference akOldContainer)
    ; Optional fallback container monitoring
EndEvent

; TODO(Phase 5b): Confirm OnRead native event behavior during CK testing
