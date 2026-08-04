ScriptName CWFramework_PlayerTalosListener extends ReferenceAlias

event OnMagicEffectApply(ObjectReference akCaster, MagicEffect akEffect)
    if akEffect
        string effectName = akEffect.GetName()
        ; Detect Blessing of Talos magic effect applied when worshipping any Shrine of Talos
        if effectName == "Blessing of Talos" || akEffect.HasKeywordString("BlessingTalos") || akEffect.HasKeywordString("ShrineTalos")
            CWFramework_TalosAwakeningQuest q = GetOwningQuest() as CWFramework_TalosAwakeningQuest
            if q
                q.OnTalosShrineWorshipped()
            endif
        endif
    endif
endEvent

event OnLocationChange(Location akOldLoc, Location akNewLoc)
    ; Listener active for Talos shrine worshipping event
endEvent
