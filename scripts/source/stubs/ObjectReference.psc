ScriptName ObjectReference extends Form hidden

ObjectReference Function PlaceAtMe(Form akFormToPlace, int aiCount = 1, bool abForcePersist = false, bool abInitiallyDisabled = false) native
Function SetPosition(float afX, float afY, float afZ) native
Function SetAngle(float afX, float afY, float afZ) native
Function Enable(bool abFadeIn = false) native
Function Disable(bool abFadeOut = false) native
Function AddItem(Form akItemToAdd, int aiCount = 1, bool abSilent = false) native
int Function GetItemCount(Form akItemToCheck) native
Function RegisterForSingleUpdate(float afTime) native
