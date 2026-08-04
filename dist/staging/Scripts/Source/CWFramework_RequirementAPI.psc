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
void Function RecordEnemyKill(string asActorBaseId, int aiCount = 1) Global Native
void Function RecordItemAbsorption(string asItemFormId, int aiCount = 1) Global Native
void Function RecordWeaponCopy(string asWeaponFormId) Global Native
void Function RecordQuestStage(string asQuestId, int aiStage) Global Native
void Function RecordPlayerLevel(int aiLevel) Global Native
void Function TriggerSpecialEvent(string asEventId) Global Native
