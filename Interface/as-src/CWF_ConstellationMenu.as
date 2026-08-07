// CWF_ConstellationMenu.as
// Top-level Scaleform Menu Class for Cardinal Weapon Framework
// Screen modes: "Selection" (first-time pick) | "Tree" (flat 2D constellation graph)

class CWF_ConstellationMenu extends MovieClip {
    public var selectionScreen: MovieClip;
    public var treeCanvas: MovieClip;
    public var infoPanel: MovieClip;

    public function CWF_ConstellationMenu() {
        super();
        trace("[CWF_UI] CWF_ConstellationMenu initialized.");
    }

    public function SetScreenMode(mode: String): Void {
        if (mode == "Selection") {
            selectionScreen._visible = true;
            treeCanvas._visible = false;
            infoPanel._visible = false;
        } else if (mode == "Tree") {
            selectionScreen._visible = false;
            treeCanvas._visible = true;
            infoPanel._visible = true;
        }
    }

    public function PopulateNodeData(nodesArray: Array): Void {
        trace("[CWF_UI] Populating 2D constellation graph with " + nodesArray.length + " nodes.");
        // Renders planar 2D orthographic node graph and parent connector lines
    }

    public function OnNodeSelected(nodeData: Object): Void {
        if (infoPanel != null) {
            infoPanel.UpdateSelectedNode(nodeData);
        }
    }
}
