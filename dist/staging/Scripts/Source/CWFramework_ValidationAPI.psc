ScriptName CWFramework_ValidationAPI Hidden

; Pre-unlock runtime verification before node selection confirmation.
bool Function VerifyNodePreUnlock(string asNodeId) Global Native

; Checks if an item/node is available or marked broken/missing.
bool Function IsItemAvailable(string asItemId) Global Native

; Returns failure reason for unavailable items.
string Function GetUnavailableReason(string asItemId) Global Native
