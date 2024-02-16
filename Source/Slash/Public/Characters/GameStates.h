#pragma once

UENUM(BlueprintType)
enum class ECharacterState :uint8 
{
	Unequipped, 
	EquippedOneHandedWeapon,
	EquippedTwoHandedWeapon
};

UENUM(BlueprintType)
enum class EActionState : uint8
{
	Default,
	HitReaction,
	Attacking,
	Equipping,
	Roll,
	Dead
};

UENUM(BlueprintType)
enum EDeathState
{
	Death1, 
	Death2,
	MAX
};


UENUM(BlueprintType)
enum class EEnemyAttackState : uint8
{
	Dead, 
	Patrolling,
	Chasing,
	Attacking,
	Engaged,
	NoState,
};