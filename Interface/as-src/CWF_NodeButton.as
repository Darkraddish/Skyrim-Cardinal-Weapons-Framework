// CWF_NodeButton.as
// Circular Node Button Component for 2D Constellation Tree

class CWF_NodeButton extends MovieClip {
    public var nodeId: String;
    public var nodeState: String; // "locked" | "unlockable" | "unlocked"
    public var iconLoader: MovieClip;
    public var padlockGlyph: MovieClip;
    public var pulseRing: MovieClip;

    public function CWF_NodeButton() {
        super();
    }

    public function SetNodeState(state: String): Void {
        nodeState = state;
        if (state == "locked") {
            // Desaturated + padlock corner glyph
            if (padlockGlyph != null) padlockGlyph._visible = true;
            if (pulseRing != null) pulseRing._visible = false;
        } else if (state == "unlockable") {
            // Colored outline pulse
            if (padlockGlyph != null) padlockGlyph._visible = false;
            if (pulseRing != null) pulseRing._visible = true;
        } else if (state == "unlocked") {
            // Solid fill + colored ring
            if (padlockGlyph != null) padlockGlyph._visible = false;
            if (pulseRing != null) pulseRing._visible = false;
        }
    }

    public function SetIcon(iconPath: String): Void {
        // Loads node icon swf/png asset
    }
}
