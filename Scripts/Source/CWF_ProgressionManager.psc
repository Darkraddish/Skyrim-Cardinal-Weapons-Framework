ScriptName CWF_ProgressionManager Extends Quest
; ==============================================================================
; Cardinal Weapon Framework — Progression Manager
; Purpose: Manages node unlocking and tracks the FOUR independent growth systems:
;          1. Evolution (Form ID)
;          2. Level (flat/additive stats)
;          3. Refinement (multiplicative crafting multiplier)
;          4. Mastery (multiplicative passive usage multiplier)
; Rule: NEVER compute or display a single combined "overall progress" score.
; Talks to: CWF_LegendaryWeaponManager, CWF_RequirementSystem, CWF_AbilitySystem
; ==============================================================================

String Property ActiveFormId Auto Hidden
Int Property WeaponLevel Auto Hidden
Float Property RefinementMultiplier Auto Hidden
Float Property MasteryMultiplier Auto Hidden

Function UnlockNode(String asNodeId)
    ; TODO(Phase 9): Evaluate node requirements via CWF_RequirementSystem, flip node state, and apply form/abilities
EndFunction

Function SetWeaponForm(String asFormId)
    ActiveFormId = asFormId
    Debug.Trace("[CWF] Active weapon form updated to: " + asFormId)
EndFunction

; Independent system tracking methods (never combined into a single score)
Int Function GetWeaponLevel()
    Return WeaponLevel
EndFunction

Float Function GetRefinementMultiplier()
    Return RefinementMultiplier
EndFunction

Float Function GetMasteryMultiplier()
    Return MasteryMultiplier
EndFunction

; TODO(Phase 3): Native SKSE queries for node graph state
