ScriptName SKI_ConfigBase Extends Quest

String Property ModName Auto
Int Property TOP_TO_BOTTOM = 1 Auto

Function SetCursorFillMode(Int mode)
EndFunction

Function AddHeaderOption(String text)
EndFunction

Int Function AddKeyMapOption(String text, Int value)
    Return 0
EndFunction

Function SetKeyMapOptionValue(Int option, Int value)
EndFunction

Function SetInfoText(String text)
EndFunction

Event OnConfigInit()
EndEvent

Event OnPageReset(String page)
EndEvent

Event OnOptionKeyMapChange(Int option, Int keycode, String conflictControl, String conflictName)
EndEvent

Event OnOptionHighlight(Int option)
EndEvent
