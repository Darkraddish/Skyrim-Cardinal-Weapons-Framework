ScriptName CWFramework_API Hidden

; Returns framework semver version
string Function GetVersion() Global Native

; Returns true if framework initialized cleanly
bool Function IsFrameworkReady() Global Native

; Returns true if 0 content packs are registered
bool Function IsZeroContentLoaded() Global Native

; Returns the total count of registered weapon types
int Function GetRegisteredWeaponTypeCount() Global Native

; Registration Native Endpoints
bool Function RegisterWeaponType(string asId, string asName, string asDescription, string asIconAsset) Global Native
bool Function RegisterSeries(string asId, string asWeaponTypeId, string asName, string asDescription, string asIconAsset) Global Native
bool Function RegisterWeaponForm(string asId, string asSeriesId, string asName, string asDescription, string asModelAsset) Global Native
bool Function RegisterProgressionNode(string asId, string asSeriesId, string asName, int aiNodeType, float afPosX, float afPosY) Global Native
