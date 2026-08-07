# Cardinal Weapon Framework — Implementation Build Guide
### Target: Skyrim Special/Anniversary Edition (SE/AE) · Full Custom Scaleform UI · Partial dev environment assumed

---

## 0. How To Use This Guide (read before doing anything)

This guide is written for two different readers working the same task list:

- **AGENT** — a coding agent with file/folder access to the mod project and the Skyrim install. The agent can create/edit text files (Papyrus `.psc`, C++, ActionScript `.as`, JSON, INI, XML), organize folders, and run command-line tools *if they exist and are documented*. The agent must never guess at a Creation Kit UI action, never invent a form ID, and never assume a compiled result is correct without a test step confirming it.
- **HUMAN** — you, the mod author. Anything involving Creation Kit's graphical editor, Flash/Animate, in-game testing, or visual judgment calls is yours. The agent cannot open a GUI application.

### 🛑 STOP markers
Every step below is tagged:

| Tag | Meaning |
|---|---|
| `[AGENT]` | Agent does this directly (writes/edits a file, runs a scriptable CLI tool). |
| `[HUMAN]` | Requires a GUI tool (Creation Kit, Flash/Animate, the game itself) or a judgment call. Agent must **stop and wait**. |
| `[AGENT→HUMAN HANDOFF]` | Agent prepares something (a spec, a stub file, a checklist) specifically so the human's GUI work has zero ambiguity. |

**Rule for the agent:** when a step is tagged `[HUMAN]`, stop, output the exact checklist for that step (already written out below — copy it verbatim), and wait for the human to reply "done" or paste back results (a FormID, a screenshot description, an error message) before continuing. Do not skip ahead. Do not attempt the GUI step yourself. Do not assume it succeeded.

### Source-of-truth documents

This guide assumes `AGENTS.md` and `CWFrameworkDesc.md` remain in the project root/`Docs/` for the lifetime of this project — they are **load-bearing**, not background reading. If anything in this guide ever conflicts with those two files: **those files win on philosophy/architecture** (what the framework is allowed to know, how systems are separated); **this guide wins on execution order** (what to build in what sequence, which steps need a human). If the agent ever hits that kind of conflict, it stops and flags it rather than silently picking one.

### 0.5 — Non-Negotiable Rules Checklist (self-check before writing *any* file)

Before the agent writes or edits a `.psc`, `.json`, `.as`, or `.cpp` file in this project, it re-checks the new content against this list. If a rule is violated, stop and flag it — don't write the file and move on.

- [ ] **No weapon-specific logic in framework code.** Framework scripts/plugin code (`Scripts/Source/CWF_*`, `SKSEPlugin/`) only ever reason in terms of Weapon Type / Series / Form / Node / Requirement / Ability. No `if weaponType == "Shield"` branches, no anime/series names, no hardcoded form lists — that all belongs in `Data/ContentPacks/`.
- [ ] **No dynamically generated progression.** Every node must already exist in the JSON before gameplay begins. "Unlocking a branch" via an EventTrigger requirement means flipping an existing hidden node's visibility — never creating a new node at runtime.
- [ ] **Parent-node dependency is the default.** A node is inaccessible until its `parentNodeId` node is completed, unless it's explicitly reached via an EventTrigger requirement (and even then, the node was pre-defined, just hidden).
- [ ] **One Legendary Weapon per save, forever.** Once `CWF_HasSelectedWeapon` flips to 1 and a weapon type is chosen, nothing in the framework may offer a way to switch it for that save.
- [ ] **Four separate progression values, never one combined score.** Evolution (Form), Level (flat/additive), Refinement (multiplicative, crafting-driven), Mastery (multiplicative, passive-use-driven) are tracked and surfaced independently — no function should compute or display a single "overall progress" number.
- [ ] **Abilities never reference weapons.** A Form references Ability IDs; an Ability definition never contains a weapon/form/series reference back. This is what keeps abilities reusable across content packs.
- [ ] **Ability scope rule.** Equip and Active abilities are form-bound — strip them on `OnUnequip`/form change. Passive and Master abilities are permanent — never strip them on form change.
- [ ] **Content packs never depend on each other.** A `ContentPacks/X/` JSON file must never reference a nodeId/requirementId/abilityId defined in `ContentPacks/Y/`.
- [ ] **A content pack is incomplete without a full tree.** Don't ship (or treat as "done") a Form with no progression path from its starting node — per `AGENTS.md`, a weapon without a progression tree isn't a valid deliverable, even a small one.
- [ ] **Validation degrades, never crashes.** Any broken/missing reference (bad `requirementId`, missing icon, etc.) marks that node/pack unavailable and continues — it never halts load, never locks the UI.
- [ ] **UI shares styling with SkyUI, not logic with vanilla perks.** The Scaleform menu may reuse SkyUI's CLIK components/fonts/colors for visual consistency, but must not read from or write to vanilla perk tree data.
- [ ] **Early-tier balance rule.** Low-tier/starting forms should have minimal or no abilities — don't front-load power just because a node happens to exist early in the tree.

---

## 1. Confirmed Decisions (from project owner)

- **Platform:** Skyrim SE/AE, current Steam version (1.6.640+ as of writing — **`[HUMAN]` confirm your exact `SkyrimSE.exe` version** via Steam properties, since SKSE builds are version-pinned).
- **UI:** Full custom Scaleform menu (Flash/Animate authoring), SkyUI-styled, flat 2D, non-3D — this is a genuine "custom menu" project, not a reskinned MCM list.
- **Dev environment:** Partially set up. Section 2 below is a gap-check checklist — human runs through it once before Phase 0 begins.

---

## 2. Toolchain Requirements — Gap Check `[HUMAN]`

Go through this list and report back which items you already have vs. still need. The agent should not proceed past Phase 0 until this list comes back confirmed.

**Core modding tools**
- [ ] Creation Kit (SE) — from Steam, Skyrim Special Edition tools
- [ ] SKSE64 — matching your exact game version (check skse.silverlock.org)
- [ ] Address Library for SKSE Plugins (required by most SKSE-dependent mods/plugins)
- [ ] SSEEdit (xEdit) — for record inspection/cleaning, conflict checks
- [ ] Mod Organizer 2 (MO2) — for isolated dev profile / load order
- [ ] A dedicated MO2 profile for this project only (empty except CWFramework + requirements), to avoid contamination from your normal load order

**Scripting**
- [ ] Papyrus Compiler (ships with Creation Kit) and the Skyrim SE base scripts (`Data/Source/Scripts`) — needed to compile `.psc` → `.pex`
- [ ] A Papyrus-aware text editor is optional but recommended (e.g., VS Code with a Papyrus extension) — agent will just write plain `.psc` text files regardless

**SKSE plugin (C++) — required because Papyrus alone cannot parse JSON or hold complex nested data structures**
- [ ] Visual Studio 2022 (Community is fine) with "Desktop development with C++" workload
- [ ] CMake
- [ ] vcpkg
- [ ] CommonLibSSE-NG (this is how modern SKSE plugins are built against SE+AE+VR simultaneously)

**MCM**
- [ ] SkyUI (the mod itself, for MCM menu framework)

**UI authoring**
- [ ] Adobe Animate (current Creative Cloud) *or* a legacy Flash CS6 install — needed to compile `.swf` Scaleform files
- [ ] The SkyUI Scaleform SDK / CLIK component source (community-maintained; needed so your custom menu can borrow SkyUI's buttons/lists/fonts and look native) — **`[HUMAN]` locate and download this before Phase 8**, agent cannot browse mod sites for you without you pointing it at a URL

**Assets (for the first content pack, Phase 10)**
- [ ] A 3D shield model + textures for the Naofumi-inspired starting shield (nif/dds), or a placeholder reused from vanilla Skyrim shields for early testing
- [ ] Icon art for each progression node (used inside the constellation UI's circular node buttons)

> **`[AGENT]` action right now:** create the project folder skeleton (Section 3) so there's a real place for all of the above to land. Do not attempt to install or download anything — flag every checkbox above back to the human as a literal checklist.

---

## 3. Project Folder Structure `[AGENT]`

Create this structure inside your mod project root (separate from the live Skyrim `Data` folder — you'll symlink/deploy via MO2, never edit files live in `Data`):

```
CardinalWeaponFramework/
├── CWFramework.esp                     (created in Phase 1, [HUMAN] via CK)
├── Scripts/
│   ├── Source/
│   │   ├── CWF_LegendaryWeaponManager.psc
│   │   ├── CWF_ProgressionManager.psc
│   │   ├── CWF_RequirementSystem.psc
│   │   ├── CWF_AbilitySystem.psc
│   │   ├── CWF_ContentValidator.psc
│   │   ├── CWF_StartQuestScript.psc
│   │   ├── CWF_BookRefScript.psc
│   │   ├── CWF_MenuHandler.psc
│   │   └── CWF_MCMScript.psc
│   └── (compiled .pex output lands here after [HUMAN] compiles in CK)
├── SKSEPlugin/
│   ├── CMakeLists.txt
│   └── src/
│       ├── main.cpp
│       ├── DataLoader.h / .cpp        (JSON schema loader — Section 4)
│       ├── InputHandler.h / .cpp      (hotkey O listener)
│       └── SaveLoad.h / .cpp          (co-save persistence)
├── Interface/
│   ├── swf/                            (compiled Scaleform output, [HUMAN])
│   └── as-src/
│       ├── CWF_ConstellationMenu.as   (agent writes source, human compiles)
│       ├── CWF_NodeButton.as
│       └── CWF_InfoPanel.as
├── Data/
│   ├── CWF_Nodes.json                 (Section 4 schema — framework-agnostic tree data)
│   ├── CWF_Requirements.json
│   ├── CWF_Abilities.json
│   └── ContentPacks/
│       └── NaofumiShield/
│           ├── NaofumiShield_Nodes.json
│           ├── NaofumiShield_Requirements.json
│           └── NaofumiShield_Abilities.json
├── MCM/
│   └── config/CWFramework/config.json  (SkyUI MCM config)
└── Docs/
    ├── STOP_CHECKPOINTS.md             (Section 9 — living checklist)
    └── DataSchema.md                   (Section 4, copied here for reference)
```

`[AGENT]`: create every folder above now, with empty placeholder files where noted, so nothing is invented later without a home.

---

## 4. Data Schema — Framework's Six Concepts as JSON `[AGENT]`

Per both design docs, the framework only knows six abstractions: **Weapon Type, Series, Form, Node, Requirement, Ability**. None of this is hardcoded — it all lives in JSON, loaded by the SKSE plugin at game load and revalidated at weapon-selection time (per the Content Validation section of `CWFrameworkDesc.md`).

```jsonc
// CWF_Nodes.json — one entry per progression node
{
  "nodeId": "shield_naofumi_start",
  "weaponType": "Shield",
  "series": "Naofumi",
  "formId": "shield_small_iron",       // references a Form entry, not a raw Skyrim FormID
  "parentNodeId": null,                // null = tree root
  "convertible": false,                // see "Convertible" node rule in CWFrameworkDesc.md
  "requirementIds": ["req_kill_10_wolves"],
  "abilityIds": [],                    // empty = no ability at this early tier, per design doc
  "effectIds": [],
  "state": "locked"                    // locked | unlockable | unlocked — runtime, not authored
}
```

```jsonc
// CWF_Requirements.json
{
  "requirementId": "req_kill_10_wolves",
  "type": "KillCount",                 // KillCount | ItemAbsorb | WeaponCopy | Quest | Level | PrevNode | Crafting | Location | EventTrigger | Custom
  "target": "Wolf",
  "amountRequired": 10
}
```

```jsonc
// CWF_Abilities.json
{
  "abilityId": "abl_iron_will_passive",
  "category": "Passive",               // Equip | Passive | Active | Master
  "scope": "Permanent",                // FormBound | Permanent — per CWFrameworkDesc.md's Abilities/Effects section
  "magicEffectFormId": "0x000801XX"    // points at an actual Skyrim MGEF once [HUMAN] creates it in CK
}
```

**`[AGENT]`** write these three schema files (with the fields above, empty arrays as examples) into `Data/` now. **Do not invent additional gameplay-specific fields** (e.g., nothing sword/shield-specific in the *framework* schema — that only appears inside `ContentPacks/`).

**`[HUMAN]`** sanity-check this schema once before Phase 3 — confirm it can represent the actual Naofumi shield progression you have in mind (rough tree sketch on paper is enough), so the SKSE loader isn't built against a schema that gets revised mid-build.

---

## 5. Build Phases

### Phase 0 — Toolchain confirmed `[HUMAN]`
Gate: Section 2 checklist fully checked off. **Stop here until you confirm.**

---

### Phase 1 — Base Plugin & Forms `[HUMAN]` (CK GUI, no way around it)

1. Open Creation Kit → File → New Plugin.
2. Save as `CWFramework.esp`.
3. Flag it ESL (File → Plugin Flags... or via SSEEdit) once it's stable — do this *last*, not now, so FormIDs don't get compressed while you're still adding records.
4. Create these base records (empty shells for now, filled in later phases):
   - Quest: `CWF_StartQuest`
   - Quest: `CWF_MainProgressionQuest` (tracks framework-wide state, non-player-facing, no stages shown in journal)
   - Global Variable: `CWF_HasSelectedWeapon` (0/1)
   - Global Variable: `CWF_MenuHotkey` (default value = DXScanCode for `O`, e.g. `24`)
   - Keyword: `CWF_LegendaryWeapon` (tag applied to any weapon that's part of this system)
   - Book: `CWF_StartingBook` — name it in-world (e.g., "A Waterlogged Tome"), placeholder text is fine for now
5. **Report back:** paste the FormIDs CK assigned to each record above. The agent needs these literal IDs for every script reference from here on — it must never guess a hex FormID.

---

### Phase 2 — Core Papyrus Scripts (skeletons) `[AGENT]`

Write these as plain script skeletons (properties + empty function stubs + comments describing intended behavior) using the FormIDs reported at the end of Phase 1. Do **not** attempt to compile yet (no compiler in the agent's environment) — that's Phase 2b.

- `CWF_LegendaryWeaponManager.psc` — registers weapon types, tracks the chosen weapon, enforces "exactly one legendary weapon per save, fixed for the save's lifetime" (per `CWFrameworkDesc.md`), save/load via `OnGameLoad`/`OnSave` (StorageUtil or SKSE `SavePersistence`).
- `CWF_ProgressionManager.psc` — unlocks forms, tracks refinement/level/mastery **as four separate tracked values**, never a single combined "progression score" (explicit rule in the design doc).
- `CWF_RequirementSystem.psc` — generic evaluator that reads `requirementId` → `type` from the SKSE-loaded data and checks it against player state. One function per `type` (KillCount, ItemAbsorb, etc.), dispatched by a string switch — **no weapon-specific branches allowed here**.
- `CWF_AbilitySystem.psc` — grants/revokes MGEF-based abilities based on `category`/`scope`; form-bound abilities (Equip/Active) get removed on `OnUnequip`, permanent ones (Passive/Master) do not.
- `CWF_ContentValidator.psc` — runs at `OnGameLoad` (full check) and again right before a node-unlock/weapon-selection confirmation is finalized (light check) — per the two-stage validation section of `CWFrameworkDesc.md`. Missing/broken content marks that node "unavailable," never crashes.
- `CWF_StartQuestScript.psc` — quest script for `CWF_StartQuest` (Phase 5 wires this up).
- `CWF_BookRefScript.psc` — attached to the `CWF_StartingBook` reference (Phase 5).
- `CWF_MenuHandler.psc` — Papyrus-side bridge that opens the Scaleform menu and pushes/receives data to it (Phase 7).
- `CWF_MCMScript.psc` — extends `SKI_ConfigBase`, exposes the hotkey rebind control (Phase 4).

`[AGENT]` output all nine files into `Scripts/Source/`, each with a header comment block stating: purpose, which FormIDs it references, and which other scripts it talks to. Stop after writing — do not invent function bodies for logic that depends on systems not built yet (e.g., don't write real Scaleform-calling code before Phase 7 exists); leave a `; TODO(Phase N): ...` comment instead.

---

### Phase 2b — First Compile Test `[HUMAN]`

1. Copy `Scripts/Source/*.psc` into your `Data/Scripts/Source/` (via MO2 overwrite or a symlinked dev mod folder — do not hand-copy into the real `Data` folder).
2. In Creation Kit: Gameplay → Compile Papyrus Scripts, or use the standalone `PapyrusCompiler.exe` from command line pointed at the Skyrim SE `Scripts/Source` import folder.
3. Confirm all nine scripts compile with **zero errors**. Skeleton stubs should compile cleanly since there's no real logic yet.
4. **Report back** any compiler errors verbatim — the agent will fix syntax based on the exact error text, not guesses.

---

### Phase 3 — SKSE Plugin (JSON data + persistence)

`[AGENT]`:
- Write `CMakeLists.txt` targeting CommonLibSSE-NG, producing `CWFramework.dll`.
- Write `DataLoader.cpp/.h`: on `SKSEMessagingInterface` `kDataLoaded` event, scan `Data/SKSE/Plugins/CWFramework/` (both the framework's own JSON and any `ContentPacks/*` subfolders dropped there by other mods — this is the "public API surface" other content packs plug into) and parse all Node/Requirement/Ability JSON into in-memory structures. Expose a small set of Papyrus native functions (e.g., `CWF_GetNodeState(string nodeId)`, `CWF_GetRequirementProgress(string reqId)`) so Papyrus scripts never touch JSON directly.
- Write `InputHandler.cpp/.h`: listens for the configurable hotkey (read from `CWF_MenuHotkey` global, default scancode for `O`), and on press, calls into `CWF_MenuHandler.psc` to open the Scaleform menu — **only if `CWF_HasSelectedWeapon` == 1** (menu shouldn't open before the intro quest finishes).
- Write `SaveLoad.cpp/.h`: hook `kSaveGame`/`kPostLoadGame` to persist progression state alongside the save (co-save), so this doesn't rely solely on Papyrus `StorageUtil` (more robust for a framework other mods depend on).

`[HUMAN]`:
1. Open the SKSE plugin folder in Visual Studio, build via CMake presets.
2. Confirm it builds with zero errors (report any errors verbatim to the agent).
3. Deploy the built `.dll` to `Data/SKSE/Plugins/` via MO2.
4. Launch the game once with just SKSE + this plugin (no other content) and confirm the game doesn't crash to desktop on load — check `Documents/My Games/Skyrim Special Edition/SKSE/CWFramework.log` (agent should have added logging) for a "plugin loaded" line.
5. **Report back:** did it load cleanly, yes/no, and paste the log's first ~20 lines.

---

### Phase 4 — MCM Hotkey Config

`[AGENT]`: write `MCM/config/CWFramework/config.json` (SkyUI MCM JSON config format) with a single key-binding control that writes to `CWF_MenuHotkey`, defaulting to `O`. Write the corresponding `CWF_MCMScript.psc` body now (this one's simple enough to fully implement, unlike the Phase 2 skeletons).

`[HUMAN]`:
1. Compile `CWF_MCMScript.psc` (same process as Phase 2b).
2. Deploy the mod via MO2 with SkyUI active, launch game, open MCM, confirm the "Cardinal Weapon Framework" page appears and the keybind control shows/rebinds correctly.
3. **Report back:** pass/fail + screenshot description if it fails.

---

### Phase 5 — Intro Quest: the Book at Farengar's `[HUMAN]`-heavy, `[AGENT]` scripts first

**`[AGENT]` first:**
- Finish `CWF_StartQuestScript.psc`: on game start (`OnPlayerLoadGame` alias event, or a `RegisterForSingleUpdate`-based check at cell load), if `CWF_HasSelectedWeapon == 0`, verify the `CWF_StartingBook` reference exists in the world (via a quest alias, not `Game.GetFormFromFile` guessing); if the quest alias is empty, do nothing and log a warning (never spawn a duplicate book from script — placement is CK's job below).
- Finish `CWF_BookRefScript.psc`: attach to the book's ObjectReference alias. On the book being read, set `CWF_HasSelectedWeapon` is **not** set here yet — reading the book should only complete `CWF_StartQuest` and reveal the weapon-selection screen; `CWF_HasSelectedWeapon` flips to 1 only once the player actually **picks** a weapon type inside that screen (Phase 7/9), per the "exactly one legendary weapon, chosen once" rule.

  ⚠️ **`[AGENT→HUMAN HANDOFF]` note:** Skyrim's Creation Kit event for a book being read on an object reference is `Event OnRead()`. This is a real but lesser-documented native event — **`[HUMAN]` verify this fires as expected during Phase 5b testing** (add a `Debug.Notification("Book was read")` line temporarily). If it does not fire, the documented fallback is: register the book reference's container (or the player's inventory) for `OnItemAdded`, then poll `(YourBookRef as Book).GetRead()` on a short `RegisterForSingleUpdate` loop until it flips true. Do not silently switch approaches — report which one actually worked.

**`[HUMAN]` — Creation Kit placement checklist:**
1. Open `whiterun.esp`/the relevant interior cell for Dragonsreach (Farengar's wing), with `CWFramework.esp` as your active plugin.
2. Place a reference of `CWF_StartingBook` on top of either Farengar's end table or his alchemy table (pick one — end table is closer to his usual seating and less likely to be knocked around by physics; alchemy table risks being obscured by his own clutter).
3. Set persistence: this reference must be **persistent** (Creation Kit: right-click reference → check "Persistent Reference") so scripts can reference it reliably across cell resets.
4. Create a Quest Alias on `CWF_StartQuest` of type "Reference Alias," and fill it with this exact book reference (via "Specific Reference" alias type, forced into the alias, or an alias condition matching the unique reference — **do not use a generic "any book of this base type" alias**, since the framework must react to *this specific* book, not every copy of the base Book form that might exist elsewhere).
5. Attach `CWF_BookRefScript.psc` to that same reference (Reference tab in CK → Scripts).
6. Set the book's base object's script property linking back to the quest alias, per the property names the agent left as stub `Properties` blocks in Phase 2.
7. **Report back:** the alias name/index CK assigned, and confirm placement visually (load the cell in-game, walk to Farengar, confirm the book is visible on the chosen table and not clipping into geometry).

---

### Phase 5b — Intro Quest Test `[HUMAN]`

1. New game or `coc whiterun` test save, walk to Dragonsreach, find and read the book.
2. Confirm the `Debug.Notification` fires (temporary test line from Phase 5).
3. Confirm `CWF_StartQuest` shows as completed (`sqs CWF_StartQuest` in console, or Papyrus log).
4. Remove the temporary debug notification once confirmed — **`[AGENT]`** does this cleanup once told it's confirmed working.

---

### Phase 6 — Weapon Selection Screen (first-time flow)

This is the very first thing the player sees inside the custom menu — it's part of the same Scaleform project as the constellation tree (Phase 8), just a different screen/state within it, shown when `CWF_HasSelectedWeapon == 0`. Note this now so Phase 8's Flash/Animate work builds both screens together rather than the human discovering a missing screen mid-build.

`[AGENT]`: add a `selection` state to the `CWF_MenuHandler.psc` bridge and to the AS source stub (Phase 8) now, even though the visuals come later — this avoids a rework later where the menu was built assuming it only ever shows the tree.

---

### Phase 7 — Hotkey → Menu Open Wiring `[AGENT]`, tested `[HUMAN]`

`[AGENT]`: finish `CWF_MenuHandler.psc` — on the SKSE `InputHandler` callback (Phase 3), call `UI.OpenMenu` / `Debug.Notification` fallback per your Scaleform integration approach (SKSE64's `RegisterForMenuOpenCloseEvent` + a custom menu name registered by the SKSE plugin). Push current node states (via the `CWF_GetNodeState`/`CWF_GetRequirementProgress` native functions from Phase 3) into the Scaleform menu's data provider on open.

`[HUMAN]`: once Phase 8's `.swf` exists (can't test this phase fully in isolation — comes back after Phase 8), confirm pressing `O` opens the menu, and that MCM-rebinding the key actually changes which key opens it.

---

### Phase 8 — Scaleform Constellation UI `[HUMAN]` (Flash/Animate authoring), `[AGENT]` provides spec + AS stubs

**`[AGENT]` deliverables before handoff** (write these into `Interface/as-src/`):
- `CWF_ConstellationMenu.as` — top-level menu class stub: two screens (`Selection`, `Tree`), a data-in function signature matching what `CWF_MenuHandler.psc` sends, and comments describing exactly which fields arrive (nodeId, state, position hints, parentNodeId for drawing connector lines).
- `CWF_NodeButton.as` — component stub for a single circular node button: takes an icon reference + state (locked/unlockable/unlocked) and exposes visual states for each (per the three-state rule in both design docs — this must be unmistakable at a glance, so each state needs a distinct look, not just a color tint).
- `CWF_InfoPanel.as` — component stub for the right-side standardized info panel: Overview Header, Selected Node Details, Requirement Display (with live `x/y` progress), Ability/Effect Display, Action Footer — exactly the five-part template both design docs specify, in that order, identical layout regardless of weapon type.

**`[AGENT→HUMAN HANDOFF]` — full visual spec, copy verbatim into your Flash/Animate project notes:**
- Canvas: flat 2D, orthographic — no perspective/3D tilt anywhere, including on hover/select (this is the explicit differentiator from vanilla's perk web).
- Layout: hard split, left ~55–60% = tree canvas, right ~40–45% = info panel, matching SkyUI's existing menu proportions so it doesn't look like a foreign overlay.
- Node visual: a circle containing the node's icon; three distinct visual states (not just opacity — e.g., locked = desaturated + padlock corner glyph, unlockable = colored outline pulse, unlocked = solid fill + colored ring) using SkyUI's existing color/font tokens where possible for consistency.
- Connector lines between parent/child nodes render as simple 2D lines/curves, not 3D tubes.
- Convertible nodes (rare, per `CWFrameworkDesc.md`) need a small distinct marker since their "unlock" is an in-place outcome swap, not a new node appearing — **`[HUMAN]` judgment call on the exact glyph, agent cannot design this.**

**`[HUMAN]` build checklist:**
1. Set up Animate/Flash project targeting the SkyUI CLIK component library (Section 2 dependency).
2. Build the two screens per the spec above, importing the AS stubs as a starting skeleton and filling in real ActionScript logic.
3. Compile to `Interface/swf/CWFramework.swf`.
4. Register the swf as a custom menu (SKSE plugin side, Phase 3's menu registration) — **`[AGENT]`** wires the registration code once told the swf's exact filename/menu-name to use.
5. Deploy, launch, press `O`, confirm the menu opens and shows *something* (even placeholder data) without crashing.
6. **Report back:** pass/fail, and if it opens, whether real node data (from the JSON test tree, Phase 9) appears correctly.

---

### Phase 9 — Wire Real Data Through the Full Stack

`[AGENT]`: once Phase 8 confirms the menu opens, populate `Data/ContentPacks/NaofumiShield/*.json` with a small real tree (5–8 nodes is enough for an end-to-end test) representing the first stretch of Naofumi's shield progression (starting Small Shield → first evolution branch). Keep every field within the schema from Section 4 — no shield-specific logic leaks into the framework's own files, only into this content pack's JSON.

`[HUMAN]`: playtest — open the menu, confirm nodes show correct locked/unlockable/unlocked states as you fulfill a test requirement (e.g., kill the required number of a test enemy), confirm the info panel's requirement counter updates live (`x/y`), confirm selecting a node and unlocking it actually changes the equipped shield's form.

---

### Phase 10 — First Content Pack: Naofumi Shield Series

This is where "managing the weapon tree like Naofumi" and "weapon upgrade systems" actually become playable, per your original request. Per `AGENTS.md`, a content pack must be a *complete* playable progression, not a placeholder — so scope Phase 10 to a real, if short, complete tree rather than a stub.

`[AGENT]`:
- Expand `NaofumiShield_Nodes.json` into a real first arc: starting Small Shield, a handful of early forms (each with modest or no abilities, per the "early-game equipment shouldn't be overpowered" rule in `AGENTS.md`), at least one branching choice, one node with a Passive ability, one with an Active ability, and one Convertible node if your design calls for one.
- Author matching `Requirements` (kill counts, item absorption, etc.) and `Abilities` JSON.
- Write out, as a plain checklist, every new Skyrim form the human needs to create in CK to back this JSON (shield NIFs per form if you have unique meshes yet, MGEFs per ability, icons per node) — do not assume form names, list them explicitly by the JSON IDs above.

`[HUMAN]`:
1. Create/import shield meshes+textures (or reuse vanilla shields as placeholders for the first pass).
2. Create the MGEFs/spells backing each Ability JSON entry.
3. Create icon assets for each node (used by `CWF_NodeButton`).
4. Full playthrough test of the arc, start to first evolution.
5. **Report back** anything that breaks, verbatim.

---

### Phase 11 — Content Validation Pass `[AGENT]` writes, `[HUMAN]` breaks it on purpose

`[AGENT]`: finish `CWF_ContentValidator.psc` and the SKSE `DataLoader` validation pass — check that every `parentNodeId`/`requirementId`/`abilityId` referenced actually resolves; anything broken gets marked unavailable, never a hard failure.

`[HUMAN]` test protocol: deliberately corrupt one field in a copy of the JSON (typo a nodeId reference), confirm the game doesn't crash and the broken node/pack is simply skipped/greyed. Then remove a content pack's JSON entirely mid-session (simulating "a pack removed mid-session," per the design doc's own example) and confirm nothing crashes.

---

### Phase 12 — Full QA Pass `[HUMAN]`

Only a human can judge "does this feel like the constellation should," so this phase has no agent component beyond fixing whatever bugs get reported. Suggested pass: new game → find book → read it → pick Shield → open tree with `O` → progress through the Naofumi arc → rebind hotkey via MCM mid-playthrough → save/reload → confirm all state persisted.

---

## 6. Master STOP-Checkpoint List (for quick reference)

1. Section 2 — toolchain gap-check confirmed.
2. Phase 1 — CK base records created, FormIDs reported.
3. Phase 2b — first script compile confirmed clean.
4. Phase 3 — SKSE plugin builds and loads without CTD, log confirmed.
5. Phase 4 — MCM page confirmed visible/functional.
6. Phase 5 — book placed, alias wired, visually confirmed in-game.
7. Phase 5b — `OnRead` behavior confirmed (or fallback polling confirmed instead).
8. Phase 8 — Scaleform menu compiles, opens, no crash.
9. Phase 9 — live data confirmed flowing menu ↔ Papyrus ↔ SKSE plugin.
10. Phase 10 — assets created, full arc playtested.
11. Phase 11 — validator confirmed to fail gracefully, twice (bad reference, missing pack).
12. Phase 12 — full QA pass.

`[AGENT]`: copy this list into `Docs/STOP_CHECKPOINTS.md` and check items off as the human confirms them, rather than assuming success from context.

---

## 7. Open Assumptions Log (flag if any of these are wrong)

- Assumed `Event OnRead()` is the correct native trigger for book-reading detection — **unverified until Phase 5b**, fallback documented above.
- Assumed CommonLibSSE-NG for the SKSE plugin (covers SE/AE/VR in one codebase) rather than a version-specific CommonLibSSE fork — confirm this matches your intended long-term compatibility target.
- Assumed the weapon-selection screen lives inside the same Scaleform project as the tree menu (two states of one menu) rather than a separate standalone menu — cheaper to build and keeps a single SkyUI-styled look, but flag if you pictured it as a totally separate interstitial (e.g., a MessageBox-driven selection instead of Scaleform).
- Assumed end table over alchemy table for book placement per the reasoning in Phase 5 step 2 — reversible with one CK edit if you'd rather it be the alchemy table.
