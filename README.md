# Gameplay Test - TDW
This is The Dark West technical interview project repository made in Unreal Engine 5.5 and based on the Top-Down template, as described in the evaluation's requirements.

### Table of Contents
- Design
    - [Architecture](./Documents/Architecture.md)
    - [Game Design](./Documents/GameDesing.md)
- External
    - [GAS Documentation](https://github.com/tranek/GASDocumentation)
    - [Lyra Sample Game](https://dev.epicgames.com/documentation/en-us/unreal-engine/lyra-sample-game-in-unreal-engine?application_version=5.0)

## Goal
Implement the [Leap Slam](https://www.poewiki.net/wiki/Leap_Slam) ability, from Path of Exile, using the Gameplay Ability System. 

In addition the project must result in a scalable and extendable prototype. As well as have documentation in the form of source code comments and descriptions of the development process in this README file.


## The Demo
The VerticalSlice scene has a player controlled character with the `GA_LeapSlam` ability bound to `Input.Skill1` which is triggered with the Q button as well as `GE_Hero_ManaRegen` gameplay effect applied through the `AS_Hero` TDWAbilitySet.

The scene also contains two dummy NPCs that can be effected by the LeapSlam ability.