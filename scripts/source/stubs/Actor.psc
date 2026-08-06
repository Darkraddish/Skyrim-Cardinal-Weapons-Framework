ScriptName Actor extends ObjectReference hidden

Function EquipItem(Form akItem, bool abPreventRemoval = false, bool abSilent = false) native
Function UnequipItem(Form akItem, bool abPreventEquip = false, bool abSilent = false) native
bool Function HasSpell(Spell akSpell) native
bool Function HasMagicEffect(MagicEffect akEffect) native
int Function GetSpellCount() native
Spell Function GetNthSpell(int n) native
