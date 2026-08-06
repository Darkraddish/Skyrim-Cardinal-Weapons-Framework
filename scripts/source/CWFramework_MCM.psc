ScriptName CWFramework_MCM extends SKI_ConfigBase

string FrameworkVersion = "v1.0.0 (Phase 1)"
string ContentPackStatus = "Cardinal Weapon Shield (Active)"

event OnConfigInit()
    ModName = "Cardinal Weapons"
    Debug.Trace("[CWFramework] MCM Config Initialized. Version: " + FrameworkVersion)
endEvent

event OnPageReset(string page)
    Debug.Trace("[CWFramework] MCM Page Reset requested. Rendering version and status info.")
    SetCursorFillMode(TOP_TO_BOTTOM)
    AddHeaderOption("Cardinal Weapons Framework")
    AddTextOption("Framework Version", FrameworkVersion, OPTION_FLAG_DISABLED)
    AddTextOption("Status", "Installed & Active", OPTION_FLAG_DISABLED)
    AddTextOption("Content Pack", ContentPackStatus, OPTION_FLAG_DISABLED)
    
    AddEmptyOption()
    AddHeaderOption("Awakening Status")
    if CWFramework_SaveAPI.IsWeaponLocked()
        AddTextOption("Awakened Weapon", CWFramework_SaveAPI.GetChosenWeaponTypeId(), OPTION_FLAG_DISABLED)
        AddTextOption("Current Form", CWFramework_UIAPI.GetCurrentFormName(), OPTION_FLAG_DISABLED)
    else
        AddTextOption("Awakened Weapon", "None (Pray at Shrine of Talos)", OPTION_FLAG_DISABLED)
        AddTextOption("Hotkey ('P')", "Awaiting Awakening", OPTION_FLAG_DISABLED)
    endif
endEvent
