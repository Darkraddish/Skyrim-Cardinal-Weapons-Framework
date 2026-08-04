# AGENTS.md

# Cardinal Weapon Framework - Agent Instructions

## Project Overview

Cardinal Weapon Framework is a Skyrim mod framework designed to create a Legendary Weapon RPG system inspired by fantasy progression systems.

The framework itself is not a weapon mod. It is an engine that allows separate content packs to add:

* Legendary weapon types
* Weapon progression trees
* Weapon forms
* Abilities
* Crafting systems
* Refinement systems
* Evolution paths
* Custom animations
* Cosmetics

The framework must remain independent from all specific weapon content.

---

# Core Design Philosophy

## Framework First

The framework must never contain:

* Sword-specific logic
* Shield-specific logic
* Bow-specific logic
* Spear-specific logic
* Anime-specific references
* Specific weapon forms
* Specific series

The framework only understands:

Weapon Types: The highest classification of a weapon within the framework, defining its core category (such as Sword, Shield, Bow, or Spear) and determining the base progression structure available to it.

Series: The thematic branch within a weapon type that defines its identity and progression style, such as Beast, Dwemer, or Dragon, shaping the visual design, abilities, and evolution path.

Forms: Specific evolutions or variations of a weapon within a series, representing distinct stages or transformations that the weapon can take as it progresses.

Nodes: Individual points within a progression tree that represent unlockable steps, each containing requirements, rewards, abilities, or branching paths to further forms.

Requirements: Conditions that must be fulfilled in order to unlock nodes, forms, or abilities, such as defeating enemies, completing quests, or absorbing items.

Abilities: Functional effects or skills granted by weapon progression, which can be passive, active, or equip-based, and define the combat identity of a weapon form.

Progression: The overall system that governs how a weapon evolves over time, including unlocking nodes, advancing forms, gaining abilities, and achieving mastery.

---

# Architecture Rules

## Data Driven Design

Do not hardcode weapon progression.

All weapon content must be represented as data.

Example hierarchy:

Weapon Type

→ Series

→ Weapon Form

→ Progression Node

→ Requirements

→ Abilities

→ Recipes

→ Effects

Adding a new weapon or expansion should require adding data, not modifying framework logic.

---

# Required Systems

## Legendary Weapon Manager

Responsible for:

* Registering weapon types
* Tracking chosen weapon
* Preventing multiple cardinal weapons
* Managing active legendary weapon state
* Handling save/load

---

## Progression Manager

Responsible for:

* Unlocking forms
* Tracking progression
* Managing refinement
* Managing mastery
* Managing evolution

---

## Requirement System

All unlock conditions must use a generic requirement system.

A progression node cannot exist in isolation or appear without context. Every node must belong to a visible progression tree, even when it is locked. Progression is always a traversal through an existing path rather than a spontaneous generation of new nodes.

The previous node in a path is a core dependency for progression. In most cases, a node cannot be accessed unless its parent node has been completed, ensuring that all progression follows a structured and traceable evolution route.

Special event-based unlocks may reveal or activate entire branches of a tree (for example, a Fitoria-type event unlocking the full Filolial Series branch). However, even in these cases, the nodes are pre-defined within the system and are simply transitioned from hidden/locked states into visible and accessible states through the event trigger.

Supported requirement examples:

* Kill count
* Item absorption
* Weapon copying
* Quest completion
* Player level
* Previous node completion (core dependency)
* Crafting requirement
* Location requirement
* Special event trigger (branch reveal/unlock)
* Custom scripted requirement

Do not create weapon-specific requirement code.

---

## Ability System

Abilities must be independent from weapons.

Supported categories:

These are categories within the Ability System that define how a skill functions when attached to a weapon form in the Cardinal Weapon Framework:

* **Equip Skill**: A persistent ability that becomes active automatically when the weapon or form is equipped. It defines the baseline behavior or identity of the weapon while in use.

* **Passive Skill**: A continuous effect that remains active as long as the weapon is equipped, without requiring player activation. It typically enhances stats, modifies combat behavior, or provides background bonuses.

* **Active Skill**: A manually triggered ability that the player can activate during gameplay. These skills usually have cooldowns or resource costs and provide situational advantages or combat techniques.

* **Master Skill**: A high-tier, advanced ability unlocked through deep progression or mastery. It represents the pinnacle of a weapon’s power and often significantly alters gameplay or unlocks unique mechanics.

A weapon form references abilities.

Abilities do not reference weapons.

---

## UI Framework

The UI system is the player-facing layer of the entire Cardinal Weapon Framework. It is not just a menu system—it is the **visual representation of weapon progression, identity, and power growth**.

Because the framework introduces an additional progression UI, it provides a dedicated interface for weapon progression that serves a similar organizational role to Skyrim’s perk interface, while remaining completely independent from the vanilla perk system internally.

---

### Weapon Overview

This is the main dashboard for the player’s chosen Legendary Weapon.

It should show:

* The currently selected weapon type (Sword, Shield, Bow, etc.)
* The active series (Beast, Dwemer, Dragon, etc.)
* Current form (e.g., Wolf Shield, Fenrir Shield)
* Overall progression status
* Mastery level or growth stage

Think of this as the “character sheet” for the weapon itself.

It answers:

> “What is my weapon right now, and how far has it evolved?”

---

### Progression Tree & Split Screen Layout

The UI uses a **Flat 2D Split-Screen Architecture** designed to blend seamlessly with **SkyUI**:

* **Left Side (Progression Tree)**: Displayed as a flat 2D node network (non-slanted perspective) where nodes and connections are laid out cleanly in a 2D planar view.
* **Right Side (Standardized Info Panel)**: Renders a uniform template across **all weapon types** (Sword, Shield, Bow, Spear, etc.).

Each node represents:

* A weapon form
* A passive upgrade
* A transformation
* A new ability
* A branching evolution path

The left pane renders:

* Locked / Unlockable / Unlocked node states
* Node connection branches and parent dependencies
* Clear visual markers for the active evolution path

The right pane renders the **standardized information template**:
1. **Overview Header**: Weapon Type, Active Series, Current Form name & icon, growth multipliers (Level, Refinement, Mastery, Effective Damage/Armor).
2. **Selected Node Details**: Node Name, Description, Type, and Unlock state.
3. **Requirement Display Panel**: Exact condition tracking and live completion progress (`7/10`).
4. **Ability Display Panel**: Name, Description, Activation Type (Equip/Passive/Active/Master), and Scope.
5. **Action Footer**: Standardized button hints (`[E] Unlock Node`, `[Tab] Back`).

---

### Node Selection

Nodes are interactive elements within the constellation.

When a player selects a node, the UI must display:

* What the node is (form, ability, upgrade, etc.)
* What it unlocks
* What it evolves into
* Whether it is currently available
* What is required to unlock it

This is the decision-making layer of the system.

It is where the player chooses:

> “Do I evolve my weapon in this direction or another?”

---

### Requirement Display

Every node has conditions, and the UI must clearly communicate them.

This section shows:

* What is required (kills, items, quests, etc.)
* Current progress toward each requirement
* What is missing
* What is already completed

The goal is clarity, not mystery.

The player should never feel confused about:

> “Why can’t I unlock this?”

Instead, they should see exactly:

> “You are 7/10 kills away from unlocking this form.”

---

### Ability Display

Each weapon form or node can grant abilities, and the UI must present them clearly.

This includes:

* Passive abilities (always active)
* Active abilities (player-triggered skills)
* Unique weapon techniques
* Master-level abilities

Each ability display should include:

* Name
* Effect description
* Activation type
* Whether it is currently unlocked or locked
* Any conditions tied to it

Abilities can be a combination of types or none at all, depending on the weapon form. Early-game or low-tier equipment—such as basic hide shields or common shields—should not necessarily grant unique or powerful skills. Instead, they may provide minimal or purely passive effects, or no abilities at all, to ensure balanced progression and prevent the player from becoming overpowered too early in the game.

This ensures the player understands not just *what they have*, but *what it does*.

---

### Progress Tracking

This system visually represents advancement over time.

It should show:

* Progress toward next node
* Mastery growth of current form
* Evolution readiness
* Completion percentage of a branch or series

This creates a sense of continuous growth, even when no new nodes are unlocked yet.

It reinforces the idea that:

> “My weapon is always evolving, even in combat.”

---

### Locked / Unlocked States

Every element in the UI must clearly communicate state.

There are three primary states:

* **Locked** – Not yet accessible
* **Unlockable** – Requirements met, can be chosen
* **Unlocked** – Already obtained and active

Visual clarity is critical here.

The player should be able to instantly understand:

* What they can use now
* What they are working toward
* What is still out of reach

This prevents confusion in complex progression trees.

---

### Visual Style Philosophy

The UI is built as a **Flat 2D UI leveraging SkyUI as a mandatory requirement**:

* Flat 2D layout (avoiding 3D slanted perk perspective)
* SkyUI-compliant panel alignment, standard font hierarchy, and dark background paneling
* Left pane for 2D tree navigation, Right pane for standardized weapon info template
* Smooth integration with Skyrim's SkyUI menu conventions

However, it must NOT depend on:

* Skyrim vanilla perk system logic
* Vanilla perk tree data
* Any hardcoded Skyrim UI behavior

This is important because:

The framework is replacing the perk system, not modifying it.

So the UI blends seamlessly with SkyUI, but is fully independent and custom-built.

---

### Core Design Goal

The UI is not just informational—it is experiential.

It should make the player feel:

* Their weapon is alive
* Their choices matter
* Their progression is visible and meaningful
* Their build is unique and evolving

In short:

> The UI is the “face” of the weapon’s evolution system, not just a menu.

---

# Content Pack Rules

A content pack must provide a complete playable progression.

A weapon without a progression tree is incomplete.

Every weapon pack must contain:

* Weapon assets
* Progression tree
* Unlock requirements
* Abilities
* Recipes
* Refinement paths
* Evolution paths
* Cosmetics
* Animations (optional)

---

# Expansion Philosophy

Expansions can extend:

## Series

Example:

Dwemer Series

Adds:

* Dwemer Sword
* Dwemer Shield
* Dwemer Bow
* Dwemer Spear

---

## Weapon Type

Example:

Shield Curse Series

Adds:

* New Shield branches
* New Shield abilities
* New Shield mechanics

---

# Dependency Philosophy

Required:

* Cardinal Weapon Framework

Optional:

* Fixed Leveling System
* Economy overhaul
* Survival overhaul
* Difficulty overhaul
* Animation packs

The framework must function without optional modules.

---

# Development Priority

Implement in this order:

1. Core framework
2. Save system
3. Weapon registration
4. Requirement engine
5. Progression engine
6. UI framework
7. First complete weapon pack
8. Expansion support
9. Animation integration

Do not create large amounts of content before the framework is stable.

---

# Coding Principles

Prioritize:

* Maintainability
* Extensibility
* Clear separation of concerns
* Reusable systems
* Data-driven design

Avoid:

* Duplicate systems
* Weapon-specific framework code
* Hardcoded progression
* Direct dependencies between expansions

The framework is the product. Content is replaceable.
