ScriptName Actor extends ObjectReference hidden

bool Function EquipItem(Form akItem, bool abPreventRemoval = false, bool abSilent = false)
    return false
EndFunction

bool Function HasMagicEffect(MagicEffect akEffect)
    return false
EndFunction

bool Function HasSpell(Spell akSpell)
    return false
EndFunction

int Function GetSpellCount()
    return 0
EndFunction

Spell Function GetNthSpell(int n)
    return None
EndFunction
