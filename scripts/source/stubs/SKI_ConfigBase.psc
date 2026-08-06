Scriptname SKI_ConfigBase extends Quest Hidden

string property ModName auto
int property OPTION_FLAG_NONE = 0 auto readOnly
int property OPTION_FLAG_DISABLED = 1 auto readOnly
int property TOP_TO_BOTTOM = 0 auto readOnly

event OnConfigInit()
endEvent

event OnPageReset(string page)
endEvent

int Function AddHeaderOption(string a_text, int a_flags = 0) Native
int Function AddTextOption(string a_text, string a_value, int a_flags = 0) Native
int Function AddEmptyOption() Native
Function SetCursorFillMode(int a_fillMode) Native
