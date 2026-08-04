# Cardinal Weapon Framework

## Framework Description

### Overview

Cardinal Weapon Framework is a modular gameplay framework for Skyrim that introduces a completely new progression system centered around **Legendary Weapons**.

Unlike traditional Skyrim gameplay, where equipment is temporary and continually replaced by stronger loot, Cardinal Weapon Framework treats a weapon as a permanent companion that grows alongside the player. Rather than searching for increasingly powerful equipment, the player develops a single chosen Legendary Weapon through progression, specialization, and long-term investment.

The framework itself contains **no playable weapon content**. It provides the systems required for other mods to define Legendary Weapons and their progression while remaining completely independent of any specific weapon, series, or theme.

If every content pack is removed, the framework should still load successfully. The game will simply contain no registered Legendary Weapons for the player to choose.

---

# Framework Philosophy

The framework follows one fundamental principle:

> **The framework provides systems. Content packs provide experiences.**

This separation exists to ensure that the framework never becomes coupled to a particular implementation. Every Legendary Weapon, regardless of its appearance or mechanics, should be built using the exact same framework systems.

For this reason, the framework never contains knowledge of:

* Individual weapons
* Weapon themes
* Weapon series
* Story-specific mechanics
* Character lore
* Visual assets
* Progression paths

Instead, the framework only understands the abstract building blocks required to construct them:

* **Weapon Type** — the top-level category a weapon belongs to (Sword, Shield, Bow, Spear...), which determines the base progression structure available to it.
* **Series** — a themed branch within a Weapon Type, shaping visual identity, abilities, and evolution path.
* **Form** — a specific stage or variation a weapon takes as it progresses within a Series.
* **Node** — a single unlockable step within a progression tree.
* **Requirement** — a condition that must be satisfied to unlock a node.
* **Ability** — a functional effect granted through progression.

Nothing more specific than these six concepts is ever known to the framework. Everything else — names, themes, lore, specific forms — is content.

These six concepts combine into a single required data hierarchy that every content pack must follow:

```
Weapon Type
 └─ Series
     └─ Weapon Form
         └─ Progression Node
             ├─ Requirements
             ├─ Abilities
             ├─ Recipes
             └─ Effects
```

Adding a new weapon or expansion should always mean adding new data along this hierarchy — never modifying the framework itself.

---

# What the Framework Provides

The framework is responsible for the infrastructure required to support every Legendary Weapon.

These responsibilities include:

* Registration of Legendary Weapon Types
* Player weapon selection
* Enforcement of exactly one Legendary Weapon per save, chosen once and fixed for the lifetime of that save — the framework does not support switching to a different cardinal weapon afterward
* Progression management
* Evolution management
* Requirement evaluation
* Ability management
* Effect management
* Data persistence
* Content validation
* User interface framework
* Public APIs for content packs

Every gameplay feature related to weapon progression passes through these systems.

The framework never directly creates gameplay content.

---

# What a Content Pack Provides

A content pack represents a complete playable implementation of one or more Legendary Weapons.

Unlike the framework, a content pack contains actual gameplay content.

This includes:

* Weapon Types
* Weapon Series
* Weapon Forms
* Progression Trees
* Progression Nodes
* Requirements
* Abilities
* Effects
* Recipes
* Visual Assets
* Audio Assets
* Animations
* Icons
* Text and Localization

A content pack should never depend on another content pack.

Each pack should provide a complete progression experience for the content it introduces. A weapon shipped without a progression tree is considered incomplete and should not be released — every weapon in a content pack must be fully playable from its starting node onward, not a partial or placeholder implementation.

---

# Legendary Weapon Progression

Every Legendary Weapon progresses through a predefined web of interconnected nodes.

Each node represents a meaningful advancement in the weapon's development.

Depending on the content pack, a node may represent:

* A new weapon form
* A permanent passive
* An equip-specific ability
* A master ability
* A refinement milestone
* A cosmetic transformation
* An evolution branch
* A special event

The framework does not distinguish between these concepts beyond their data definitions. To the framework, every progression step is simply a node with associated data.

A node is not required to depend on a single condition. Some nodes — particularly those representing a Legendary Weapon's higher-tier forms — require several conditions to be satisfied together before they become available. The framework treats this as the normal case, not an exception.

---

# The Progression Web

The framework represents progression as a radial node graph.

Every Legendary Weapon begins at a central starting node.

From this point, progression expands outward through multiple branching paths.

Each branch represents a distinct Series.

Each node belongs to exactly one progression tree and maintains explicit relationships with its parent and child nodes.

The framework never generates progression dynamically.

Every node exists before gameplay begins.

Player progression consists entirely of traversing this predefined structure.

Hidden content is revealed by changing node visibility rather than creating new nodes.

In most cases, this traversal is strictly sequential — a node becomes available only once its parent has been completed. A small number of nodes may instead be marked **Convertible**: rather than the player advancing to a new node, that node's own outcome is replaced in place once a later condition is met. This is reserved for cases where a node's initial outcome represents an incomplete or provisional result of the path that reached it, and is later resolved into its final form. It is not a general substitute for ordinary progression, which remains the default for all standard evolution paths.

---

# Separation of Progression Systems

Weapon growth is intentionally divided into multiple independent systems.

Each system represents a different aspect of progression and is tracked separately.

These systems are:

**Evolution**

Determines which weapon form the player currently possesses, by traversing the progression web node by node. This is the only one of the four systems that changes the weapon's identity rather than its stats.

**Level**

Provides fixed, additive statistical growth — a flat increase applied on top of a form's base stats.

**Refinement**

Provides multiplicative improvement through crafting and enhancement, in the same spirit as Skyrim's own weapon quality tiers. Refinement scales a form's stats rather than replacing them.

**Mastery**

Represents long-term familiarity gained through continuous use of the current weapon form. Mastery grows passively through play rather than through node unlocks, and acts as its own multiplier — either strengthening a form's stats generally, or amplifying the effect of its Passive abilities specifically.

These four systems are designed to complement one another rather than replace one another. A weapon's true power at any moment is the combined result of all four working together — its current Form (Evolution), its flat bonuses (Level), and two layered multipliers (Refinement and Mastery).

**No single value should represent overall weapon progression.** Each system is tracked and surfaced to the player independently.

---

# Abilities and Effects

Progression grants the player two distinct kinds of outcomes: **Abilities**, which are functional and often activated, and **Effects**, which are passive modifiers with no activation of their own.

**Abilities** fall into four categories, each with a fixed scope:

* **Equip** and **Active** abilities are *form-bound* — they only function while that specific weapon form is currently equipped, and stop working the moment the player moves to a different form.
* **Passive** and **Master** abilities are *permanent* — once unlocked, they remain with the player regardless of which form is currently active.

**Effects** may be positive or negative, and a single node or form is free to grant both at once — a stat boost alongside a drawback, for example — the same way a single enchantment in vanilla Skyrim can carry a benefit and a cost together.

Abilities and weapons are related in one direction only: a weapon form references the abilities it grants, but an ability never references a specific weapon back. This keeps every ability fully reusable — the same ability definition can be granted by any number of unrelated weapon forms across any number of content packs.

Neither Abilities nor Effects are ever hardcoded to a specific weapon. Both are attached to nodes entirely through content pack data.

---

# Content Validation

Because every piece of gameplay content originates from external content packs, the framework treats content as untrusted until verified.

Validation happens in two stages: once when a save loads, checking every registered pack's data for structural integrity, and again — more lightly — at the moment the player is about to select a weapon or confirm a node unlock, to catch anything that changed since load (a toggled mod, a pack removed mid-session).

This second check is deliberately allowed a brief moment to run before a selection is finalized, rather than resolving instantly. A broken or missing piece of content should never crash the game or lock the player out of the interface — it should simply be marked unavailable, while everything else continues to function normally.

---

# User Interface

The interface is the player-facing layer of the entire framework — the way progression is actually seen and understood, not just tracked internally. It replaces Skyrim's perk menu entirely, while remaining fully independent from it.

It's built around a few core pieces:

* **Weapon Overview** — a dashboard showing the player's current Weapon Type, Series, Form, and overall progression at a glance. This answers "what is my weapon right now, and how far has it evolved?"
* **The Progression Web** — the radial node map itself (see above), where the player explores and selects nodes.
* **Node Selection** — selecting a node shows what it is, what it unlocks, what it evolves into, and whether it's currently available. This is where the player decides which direction to grow in.
* **Requirement Display** — every node's conditions are shown clearly, along with live progress toward each one, so the player always understands exactly what's missing rather than guessing.
* **Ability Display** — each granted ability is shown with its name, effect, activation type, and scope, so the player understands not just what they have, but what it actually does and when it applies.

Every node is always in one of three states — **Locked**, **Unlockable**, or **Unlocked** — and the interface must make that state unmistakable at a glance.

Visually, the interface leans into a dark, mystical, arcane aesthetic with glowing connections radiating outward from the center — familiar in tone to a perk-style UI, but built entirely from scratch, sharing no logic or data with Skyrim's own perk system. The goal is for the player to feel that their weapon is alive, that their choices matter, and that progression is always visible — not just a menu, but the face of the weapon's evolution.

The framework intentionally avoids making gameplay decisions.

Examples include:

* Economy balance
* Survival mechanics
* Character leveling philosophy
* Combat difficulty
* Animation replacements

These concerns belong to optional companion mods.

The framework must remain fully functional regardless of whether any companion modules are installed.

---

# Expansion Philosophy

Content is allowed to grow in exactly two directions, both of which add data rather than touching framework code:

* **Extending a Series** — introducing a themed branch across multiple Weapon Types at once. A "Dwemer" expansion, for example, could add a Dwemer Sword, Dwemer Shield, Dwemer Bow, and Dwemer Spear together, all sharing the same thematic identity.
* **Extending a Weapon Type** — introducing new branches, abilities, and mechanics within a single Weapon Type. A "Shield Curse" expansion, for example, could add new Shield-specific series and mechanics without touching any other Weapon Type.

Both are just new data flowing into the same hierarchy every other content pack already uses.

---

# Modularity

The project is designed around independent modules.

```
Cardinal Weapon Framework
│
├── Base Cardinal Heroes
│
├── Series Expansion Packs
│
├── Weapon Expansion Packs
│
└── Optional Companion Modules
```

Each module has a clearly defined responsibility.

The framework provides systems.

Content packs provide progression.

Companion modules modify gameplay.

No module should assume the presence of another optional module.

---

# Long-Term Vision

Cardinal Weapon Framework is intended to become a reusable platform for creating Legendary Weapon experiences.

The official implementation may provide the four Cardinal Weapons, but the framework itself is not limited to those weapons.

Future content creators should be able to introduce entirely new Legendary Weapon Types, progression trees, and gameplay styles without modifying framework code.

If a new Legendary Weapon can be implemented entirely through data and content pack definitions, the framework has achieved its design goal.