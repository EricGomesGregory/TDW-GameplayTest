- [README](../README.md)

# Game Desing
This section describes the game design considerations of the Leap Slam ability.

## Leap Slam
When designing the Leap Slam ability I had modulaity in mind. Early  on I split the ability into two elements, the Leap and the Slam.

The Leap ability is a simple C++ focused ability that makes a character leap towards a point that is determined either by the PlayerController for player controlled characters or by its facing direction for NPCs.

The Slam ability is a hybrid C++ and Blueprint ability that is triggered when the character lands from its leap, applying knockback and gameplay effects to actors within range of the attack.

To implement the slam in a modular way I borrowed a concept from the ttrpg [Pathfinder](https://paizo.com/pathfinder) known as [Emanation](https://2e.aonprd.com/Rules.aspx?ID=2387) that resulted in a Emanation ability that handles detecting actors within a radius and applying gameplay effects.

This ability was used to make the Slam ability but could also be used to make aura-like effects or even explosive enemies with abilities.

> ### In Retrospect
> Maybe in the future, if I were to reimplement this ability I might make the Emanation int a AbilityTask as well as the Knockback.
>