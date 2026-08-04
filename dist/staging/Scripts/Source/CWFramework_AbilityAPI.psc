ScriptName CWFramework_AbilityAPI Hidden

; Triggers a form-bound active skill.
; Returns true if skill is active and successfully triggered.
bool Function TriggerActiveSkill(string asAbilityId) Global Native

; Returns true if a specific ability ID is currently active (form-bound or permanent).
bool Function IsAbilityActive(string asAbilityId) Global Native

; Returns the net stat modifier value for a target Actor Value (e.g. +15.0 for Health boost).
float Function GetNetActorValueModifier(string asActorValue) Global Native
