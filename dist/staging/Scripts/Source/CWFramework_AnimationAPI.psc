ScriptName CWFramework_AnimationAPI Hidden

; Returns active weapon form ID condition variable for Open Animation Replacer (OAR) / DAR.
string Function GetOARWeaponFormConditionVariable() Global Native

; Returns locked weapon type ID condition variable for Open Animation Replacer (OAR) / DAR.
string Function GetOARWeaponTypeConditionVariable() Global Native

; Triggers visual transformation particle and sound effects when a weapon form evolves.
Function TriggerTransformationEffects(string asNewFormId) Global Native
