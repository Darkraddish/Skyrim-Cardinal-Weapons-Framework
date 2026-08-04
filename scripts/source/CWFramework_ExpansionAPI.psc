ScriptName CWFramework_ExpansionAPI Hidden

; Registers an optional companion module (0 = Economy, 1 = Survival, 2 = Leveling, 3 = Animation).
Function RegisterCompanionModule(int aiModuleType, string asModuleName) Global Native

; Unregisters a companion module when disabled/uninstalled.
Function UnregisterCompanionModule(int aiModuleType) Global Native

; Returns true if a specific companion module is currently active.
bool Function IsCompanionModuleActive(int aiModuleType) Global Native
