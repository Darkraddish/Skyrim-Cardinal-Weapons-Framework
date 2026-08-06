ScriptName SKI_ConfigBase extends Quest hidden

string[] Property Pages Auto
string Property ModName Auto

int Property TOP_TO_BOTTOM Auto
int Property OPTION_FLAG_NONE Auto
int Property OPTION_FLAG_DISABLED Auto

Function SetCursorFillMode(int mode)
EndFunction

int Function AddHeaderOption(string text, int flags = 0)
    return 0
EndFunction

int Function AddTextOption(string text, string val, int flags = 0)
    return 0
EndFunction

int Function AddEmptyOption()
    return 0
EndFunction

event OnConfigInit()
endEvent

event OnPageReset(string page)
endEvent
