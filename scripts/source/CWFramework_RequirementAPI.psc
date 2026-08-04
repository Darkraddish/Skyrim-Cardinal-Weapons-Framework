ScriptName CWFramework_RequirementAPI Hidden

; Evaluates whether a progression node's parent dependencies and requirements are met.
bool Function CanUnlockNode(string asNodeId) Global Native

; Returns current progress value toward a specific requirement (e.g. 7.0 for 7 kills).
float Function GetRequirementCurrentProgress(string asReqId) Global Native

; Returns target requirement value (e.g. 10.0 for 10 kills).
float Function GetRequirementTargetValue(string asReqId) Global Native

; Returns true if a specific requirement ID is completely met.
bool Function IsRequirementMet(string asReqId) Global Native

; Event Recording Endpoints
Function RecordEnemyKill(string asActorBaseId, int aiCount = 1) Global Native
Function RecordItemAbsorption(string asItemFormId, int aiCount = 1) Global Native
Function RecordWeaponCopy(string asWeaponFormId) Global Native
Function RecordQuestStage(string asQuestId, int aiStage = 0) Global Native
Function RecordPlayerLevel(int aiLevel) Global Native
Function TriggerSpecialEvent(string asEventId) Global Native
