ScriptName CWF_RequirementSystem Extends Quest
; ==============================================================================
; Cardinal Weapon Framework — Requirement System
; Purpose: Generic requirement evaluator matching requirementId -> type against
;          player state. Supports KillCount, ItemAbsorb, WeaponCopy, Quest, Level,
;          PrevNode, Crafting, Location, EventTrigger, Custom.
; Rule: Absolutely NO weapon-specific branches allowed here.
; Talks to: SKSE DataLoader, CWF_ProgressionManager
; ==============================================================================

Bool Function EvaluateRequirement(String asReqId)
    ; String switch evaluation based on SKSE requirement schema data
    ; Returns True if requirement condition is satisfied
    Return True
EndFunction

Int Function GetRequirementProgress(String asReqId)
    ; Returns current player count/progress towards requirement (e.g. 7 in 7/10)
    Return 0
EndFunction

Int Function GetRequirementTargetAmount(String asReqId)
    ; Returns required target amount (e.g. 10 in 7/10)
    Return 0
EndFunction

; TODO(Phase 3): Wire SKSE native bindings for requirement data lookups
