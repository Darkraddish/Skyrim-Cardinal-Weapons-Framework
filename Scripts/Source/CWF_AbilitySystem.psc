ScriptName CWF_AbilitySystem Extends Quest
; ==============================================================================
; Cardinal Weapon Framework — Ability System
; Purpose: Manages application and removal of MagicEffect/Spell based abilities.
; Scope Rule:
;   - Equip / Active skills are FormBound (stripped on unequip or form change)
;   - Passive / Master skills are Permanent (retained permanently)
; Dependency Rule: Abilities NEVER reference weapons. Forms reference Abilities.
; Talks to: CWF_ProgressionManager
; ==============================================================================

Function ApplyAbility(String asAbilityId)
    ; Adds ability spell/MGEF to player based on ability definition
EndFunction

Function RemoveFormBoundAbilities()
    ; Strips all FormBound (Equip/Active) abilities when changing weapon forms
EndFunction

Function RefreshAbilities()
    ; Re-evaluates equipped form abilities
EndFunction

; TODO(Phase 3): Native SKSE queries for magic effect resolution
