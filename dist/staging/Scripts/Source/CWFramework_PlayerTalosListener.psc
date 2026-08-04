ScriptName CWFramework_PlayerTalosListener extends ReferenceAlias

event OnInit()
    RegisterForSingleUpdate(2.0)
endEvent

event OnPlayerLoadGame()
    RegisterForSingleUpdate(2.0)
endEvent

event OnMagicEffectApply(ObjectReference akCaster, MagicEffect akEffect)
    if akEffect
        string effectName = akEffect.GetName()
        if effectName == "Blessing of Talos" || akEffect.HasKeywordString("BlessingTalos") || akEffect.HasKeywordString("ShrineTalos")
            TriggerAwakeningQuest()
        endif
    endif
endEvent

event OnActivate(ObjectReference akItemObject)
    if akItemObject
        string name = akItemObject.GetName()
        if name == "Shrine of Talos" || akItemObject.HasKeywordString("ShrineTalos") || name == "ShrineTalos"
            TriggerAwakeningQuest()
        endif
    endif
endEvent

Function TriggerAwakeningQuest()
    CWFramework_TalosAwakeningQuest q = GetOwningQuest() as CWFramework_TalosAwakeningQuest
    if q
        q.OnTalosShrineWorshipped()
    endif
EndFunction
