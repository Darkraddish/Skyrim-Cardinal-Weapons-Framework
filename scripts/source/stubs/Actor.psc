Scriptname Actor extends ObjectReference Hidden

bool Function HasSpell(Form akSpell) Native
bool Function HasMagicEffect(MagicEffect akEffect) Native
Function EquipItem(Form akItem, bool abPreventRemoval = false, bool abSilent = false) Native
Function AddSpell(Form akSpell, bool abVerbose = true) Native
