# Cardinal Weapon Framework - Development & Implementation Plan

> **Framework Vision**: A modular Skyrim gameplay framework providing systems for Legendary Weapon progression, completely decoupled from specific content packs.

---

## Overall Project Progress Overview

- **Current Status**: Phase 5 Completed - Progression & Sub-System Engines
- **Overall Completion**: `45%`
- **Total Phases**: 11
- **Completed Phases**: 5

| Phase | Phase Name | Status | Target Deliverables |
| :--- | :--- | :---: | :--- |
| **Phase 1** | Core Architecture & Data Schemas | `[x] Completed` | Data models, schemas, and public framework APIs |
| **Phase 2** | Save System & Data Persistence | `[x] Completed` | Single-weapon lock per save & state serialization |
| **Phase 3** | Legendary Weapon Manager | `[x] Completed` | Weapon type registration & active weapon lifecycle |
| **Phase 4** | Requirement Engine | `[x] Completed` | Generic condition evaluation & live progress tracking |
| **Phase 5** | Progression & Sub-System Engines | `[x] Completed` | Evolution, Level, Refinement, & Mastery engines |
| **Phase 6** | Ability & Effect Engine | `[ ] Pending` | Equip, Active, Passive, & Master skill handlers |
| **Phase 7** | Content Validation Engine | `[ ] Pending` | Two-stage load/runtime integrity & failure isolation |
| **Phase 8** | Custom Constellation UI Framework | `[ ] Pending` | Standalone radial constellation menu & dashboards |
| **Phase 9** | Reference Content Pack | `[ ] Pending` | First fully playable weapon pack (Sword, Shield, Bow, Spear) |
| **Phase 10** | Expansion Support & Companion Hooks | `[ ] Pending` | Series & Weapon Type expansion data pipelines |
| **Phase 11** | Animation & Cosmetic Integration | `[ ] Pending` | OAR/DAR animation hooks & visual transformation pipeline |

---

## Detailed Phase Breakdown

### Phase 1: Core Architecture & Data Schemas
**Status**: `[x] Completed`

* **Goal**: Establish the decoupled, data-driven architecture and schema hierarchy.
* **Key Guidelines & Rules**:
  * The framework must never contain weapon-specific or series-specific logic.
  * Define strict data models for the 6 core hierarchy levels: `Weapon Type` → `Series` → `Weapon Form` → `Progression Node` → (`Requirements` / `Abilities` / `Recipes` / `Effects`).
  * Ensure the framework loads cleanly with 0 content packs installed.
* **Tasks**:
  - [x] Define JSON / Papyrus / C++ Data Schemas for Weapon Types, Series, Forms, and Nodes.
  - [x] Create abstract interfaces for Requirements, Abilities, Recipes, and Effects.
  - [x] Implement Public API endpoints for content pack registration.
  - [x] Verify framework zero-content loading behavior.

---

### Phase 2: Save System & Persistence Engine
**Status**: `[x] Completed`

* **Goal**: Manage persistent state across play sessions and enforce single-weapon choice.
* **Key Guidelines & Rules**:
  * Enforce **exactly one** Legendary Weapon per save file (chosen once, permanent for save lifetime).
  * Persist node unlock states, current form, and sub-system values across save loads.
* **Tasks**:
  - [x] Build Save/Load serializer (SKSE CoSave / Papyrus persistence).
  - [x] Implement single-weapon lock-in flag per character save.
  - [x] Store active weapon state, unlocked node array, and branch visibility flags.
  - [x] Implement save file validation to prevent duplicate or illegal weapon state.

---

### Phase 3: Legendary Weapon Manager & Registration
**Status**: `[x] Completed`

* **Goal**: Manage registration and active lifecycle of Legendary Weapons.
* **Key Guidelines & Rules**:
  * Handle content pack weapon registrations dynamically at startup.
  * Track player's active chosen weapon and equip/unequip events.
* **Tasks**:
  - [x] Implement `LegendaryWeaponManager` for registering Weapon Types from content packs.
  - [x] Build player weapon selection handler and choice confirmation flow.
  - [x] Manage active form swap logic on weapon draw/equip.
  - [x] Prevent player from equipping multiple cardinal weapons simultaneously.

---

### Phase 4: Requirement Engine
**Status**: `[x] Completed`

* **Goal**: Provide a generic, expandable unlock condition evaluation system.
* **Key Guidelines & Rules**:
  * No weapon-specific requirement code allowed.
  * Every node must belong to a visible progression tree and enforce core parent node dependency.
  * Support live progress calculations (e.g. 7/10 kills) for UI feedback.
* **Tasks**:
  - [x] Implement generic `RequirementEvaluator` module.
  - [x] Build condition handlers for:
    - [x] Kill Count Tracker
    - [x] Item Absorption / Ingestion Listener
    - [x] Weapon Copying Mechanic Check
    - [x] Quest & Location Requirements
    - [x] Player Level Requirement
    - [x] Core Parent Node Dependency Check
    - [x] Special Event Triggers (branch reveal/unlock)
    - [x] Custom Scripted Requirements
  - [x] Create live progress reporting API for UI display.

---

### Phase 5: Progression & Sub-System Engines
**Status**: `[x] Completed`

* **Goal**: Manage radial progression web traversal and the four independent growth sub-systems.
* **Key Guidelines & Rules**:
  * Never aggregate growth into a single unified level value; track all 4 sub-systems separately.
  * **Evolution**: Web traversal & form identity changes (including convertible node support).
  * **Level**: Flat additive statistical growth.
  * **Refinement**: Multiplicative crafting/enhancement quality scaling.
  * **Mastery**: Passive continuous usage multiplier.
* **Tasks**:
  - [x] Implement `EvolutionEngine` for node traversal, path unlocking, and convertible node resolution.
  - [x] Implement `LevelEngine` for flat bonus application.
  - [x] Implement `RefinementEngine` for crafting enhancement scaling.
  - [x] Implement `MasteryEngine` for tracking weapon usage and passive multiplier growth.
  - [x] Build node state manager (Locked, Unlockable, Unlocked).

---

### Phase 6: Ability & Effect Engine
**Status**: `[ ] Pending`

* **Goal**: Manage skills, passive modifiers, and positive/negative stat effects.
* **Key Guidelines & Rules**:
  * One-way binding: Weapon forms reference abilities; abilities NEVER reference weapons.
  * Ability Scopes:
    * `Equip` & `Active`: Form-bound (only active while that form is equipped).
    * `Passive` & `Master`: Permanent (remain active across all forms once unlocked).
  * Effects: Support simultaneous positive boosts and negative drawbacks on a single node/form.
* **Tasks**:
  - [ ] Build `AbilityManager` handling Equip, Active, Passive, and Master skill categories.
  - [ ] Implement form-bound listener (activates/deactivates Equip/Active skills on form change).
  - [ ] Implement permanent ability manager (persists Passive/Master skills across forms).
  - [ ] Implement positive & negative effect application engine.

---

### Phase 7: Content Validation Engine
**Status**: `[ ] Pending`

* **Goal**: Protect gameplay stability against missing or broken content packs.
* **Key Guidelines & Rules**:
  * Content is treated as untrusted until verified.
  * Two-stage validation: Load-time structural check + Pre-unlock/selection verification.
  * Graceful degradation: Mark missing content unavailable without crashing or locking UI.
* **Tasks**:
  - [ ] Build Stage 1 Load-Time Validator (verifies schema structure, node parent links, asset references).
  - [ ] Build Stage 2 Pre-Unlock Runtime Validator (lightweight check prior to choice confirmation).
  - [ ] Build soft-failure fallback handler to bypass missing nodes safely.

---

### Phase 8: Custom Constellation UI Framework
**Status**: `[ ] Pending`

* **Goal**: Deliver a standalone, perk-style radial constellation interface.
* **Key Guidelines & Rules**:
  * Completely independent of vanilla Skyrim perk menu logic.
  * Dark, mystical, arcane aesthetic with glowing constellation connections.
* **Tasks**:
  - [ ] Construct custom UI framework overlay.
  - [ ] Build **Weapon Overview Dashboard** (Weapon Type, Series, Form, growth stats).
  - [ ] Build **Radial Progression Constellation Map** (node network, spatial clustering, path lines, Locked/Unlockable/Unlocked states).
  - [ ] Build **Node Selection & Interaction Layer** (decision-making, path previews).
  - [ ] Build **Requirement Display Panel** (exact condition tracking & live progress).
  - [ ] Build **Ability Display Panel** (name, description, activation type, scope).

---

### Phase 9: Reference Content Pack (First Complete Weapon Pack)
**Status**: `[ ] Pending`

* **Goal**: Provide the first fully playable implementation of Legendary Weapons.
* **Key Guidelines & Rules**:
  * Provide complete, non-placeholder progression trees for starting weapons (Sword, Shield, Bow, Spear).
* **Tasks**:
  - [ ] Create content pack data structure adhering to the required framework hierarchy.
  - [ ] Define initial weapon forms, series, and radial progression constellation trees.
  - [ ] Implement unlock requirements, abilities, recipes, and refinement paths.
  - [ ] Perform full end-to-end playability test from starting node to capstone forms.

---

### Phase 10: Expansion Support & Companion Hooks
**Status**: `[ ] Pending`

* **Goal**: Enable seamless extension via series/weapon packs and companion modules.
* **Key Guidelines & Rules**:
  * Extension directions: Extend Series (cross-weapon theme) or Extend Weapon Type (deep series expansion).
  * Framework remains fully functional without optional companion modules (Economy, Survival, Leveling, Animation).
* **Tasks**:
  - [ ] Validate multi-pack loading and series extension data merger.
  - [ ] Create hook APIs for companion modules (Economy, Survival, Leveling).
  - [ ] Test modular decoupling (enable/disable companion modules without framework breakage).

---

### Phase 11: Animation & Cosmetic Integration
**Status**: `[ ] Pending`

* **Goal**: Integrate custom animation triggers and visual form transformation effects.
* **Key Guidelines & Rules**:
  * Maintain optional dependency model for animation engines (OAR/DAR).
* **Tasks**:
  - [ ] Implement Open Animation Replacer (OAR) / DAR condition hooks for weapon forms.
  - [ ] Build cosmetic transformation particle & sound effect trigger system.
  - [ ] Verify visual form updates on evolution unlock.

---

## Recommended Execution Order

```
[Phase 1: Architecture & Schemas]
         │
         ▼
[Phase 2: Save System] ──► [Phase 3: Weapon Manager]
                                    │
                                    ▼
                          [Phase 4: Requirement Engine]
                                    │
                                    ▼
                          [Phase 5: Progression Engine]
                                    │
                                    ▼
                          [Phase 6: Ability & Effect Engine]
                                    │
                                    ▼
                          [Phase 7: Content Validation]
                                    │
                                    ▼
                          [Phase 8: Constellation UI]
                                    │
                                    ▼
                          [Phase 9: Reference Content Pack]
                                    │
                                    ▼
                          [Phase 10: Expansion Support] ──► [Phase 11: Animations]
```
