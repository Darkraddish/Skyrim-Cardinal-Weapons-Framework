ScriptName ObjectReference extends Form hidden

ObjectReference Function PlaceAtMe(Form akFormToPlace, int aiCount = 1, bool abForcePersist = false, bool abInitiallyDisabled = false)
    return None
EndFunction

Function SetPosition(float afX, float afY, float afZ)
EndFunction

Function SetAngle(float afX, float afY, float afZ)
EndFunction

Function Enable(bool abFadeIn = false)
EndFunction

Function Disable(bool abFadeOut = false)
EndFunction

Function AddItem(Form akItemToAdd, int aiCount = 1, bool abSilent = false)
EndFunction

int Function GetItemCount(Form akItemToCheck)
    return 0
EndFunction
