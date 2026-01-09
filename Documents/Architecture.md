[README](../README.md)

# Architecture
This section describes the [Lyra Sample Game](https://dev.epicgames.com/documentation/en-us/unreal-engine/lyra-sample-game-in-unreal-engine?application_version=5.0) inspired architecture for this project.

## Ability System

**TDWAbilitySystemComponent**
- Ability system component derived class for the project

### Abilities

**TDWGameplayAbility**
- GameplayAbility derived class
- Abstract base class for all project gameplay abilities
- Defines how abilities are triggered (InputTrigger, InputHeld, or OnSpawn)

### Attributes

**TDWAttributeSet**
- AttributeSet derived class
- Abstract base class for all project attribute sets
- Defines the `ATTRIBUTE_ACCESSORS` macro


**TDWHealthSet**
- Attribute set for handling health/armor/shields as well as healing and damage
- Defines Health and MaxHealth protected attributes with public accessors
- Defines Damage and Healing meta-attributes

**TDWCombatSet**
- Attribute set for combat attributes such as attack speed, etc
- Defines AttackSpeed protected attribute with public accessors

**TDWManaSet**
- Attribute set for handling mana as well as spending and recovering it
- Defines Mana and MaxMana protected attributes with public accessors
- Defines Spending and Recovering meta-attributes


___

```mermaid
classDiagram

TDWAttributeSet --|> TDWHealthSet

TDWAttributeSet --|> TDWCombatSet

TDWAttributeSet --|> TDWManaSet


class TDWAttributeSet {
    GetWorld()
    GetTDWAbilitySystemComponent()
}

class TDWHealthSet {
    + Health
    + MaxHealth
    + Damage
    + Healing
}

class TDWCombatSet {
    + AttackSpeed
}

class TDWManaSet {
    + Mana
    + MaxMana
    + Spending
    + Recovering
}

```

## Character

## Input

**TDWInputConfig**
- DataAsset derived class
- *TDWInputAction* struct. Defines relationship between InputAction and GameplayTag
- Container for NativeInputActions and AbilityInputActions

**TDWInputComponent**
- EnhancedInputComponent derived class
- Handles binding native and ability actions to functions based on 

## Player

**TDWPlayerController**
- PlayerController derived class
- Handles player character control and triggering abilities for them

**TDWPlayerState**
- PlayerState derived class
- Container for the player's TDWAbilitySystemComponent (AbilitySystemComponent), TDWHealthSet (HealthSet), TDWCombatSet (CombatSet) and TDWManaSet (ManaSet) attribute sets