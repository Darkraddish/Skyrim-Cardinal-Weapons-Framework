// CWF_InfoPanel.as
// Standardized 5-Section Details Panel (Right side of flat 2D split-screen UI)
// Standard layout across ALL weapon types (Sword, Shield, Bow, Spear)

class CWF_InfoPanel extends MovieClip {
    // 1. Overview Header
    public var weaponTypeField: TextField;
    public var activeSeriesField: TextField;
    public var currentFormNameField: TextField;
    public var levelField: TextField;
    public var refinementField: TextField;
    public var masteryField: TextField;

    // 2. Selected Node Metadata
    public var nodeNameField: TextField;
    public var nodeDescField: TextField;

    // 3. Requirement Display Panel (live progress x/y)
    public var requirementTextField: TextField;

    // 4. Ability & Effect Display Panel
    public var abilityNameField: TextField;
    public var abilityTypeField: TextField; // Equip | Passive | Active | Master
    public var abilityScopeField: TextField; // FormBound | Permanent

    // 5. Action Footer
    public var actionFooterText: TextField;

    public function CWF_InfoPanel() {
        super();
    }

    public function UpdateSelectedNode(data: Object): Void {
        if (data == null) return;

        // 1. Header
        if (weaponTypeField) weaponTypeField.text = data.weaponType;
        if (activeSeriesField) activeSeriesField.text = data.series;
        if (currentFormNameField) currentFormNameField.text = data.formName;

        // 2. Metadata
        if (nodeNameField) nodeNameField.text = data.nodeName;
        if (nodeDescField) nodeDescField.text = data.nodeDesc;

        // 3. Requirement (e.g. 7/10)
        if (requirementTextField) {
            requirementTextField.text = data.reqText + " (" + data.reqCurrent + "/" + data.reqTarget + ")";
        }

        // 4. Ability & Scope
        if (abilityNameField) abilityNameField.text = data.abilityName;
        if (abilityTypeField) abilityTypeField.text = "Category: " + data.abilityCategory;
        if (abilityScopeField) abilityScopeField.text = "Scope: " + data.abilityScope;

        // 5. Action Footer
        if (actionFooterText) {
            if (data.state == "unlockable") {
                actionFooterText.text = "[E] Unlock Node    [Tab] Back";
            } else if (data.state == "unlocked") {
                actionFooterText.text = "[Unlocked]    [Tab] Back";
            } else {
                actionFooterText.text = "[Locked]    [Tab] Back";
            }
        }
    }
}
