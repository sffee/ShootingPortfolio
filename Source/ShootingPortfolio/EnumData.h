#pragma once

UENUM(BlueprintType)
enum class ECrosshairType : uint8
{
	Cross,

	MAX
};

UENUM(BlueprintType)
enum class EPlayerState : uint8
{
	Idle,
	Reloading,

	MAX
};

UENUM(BlueprintType)
enum class ETurnState : uint8
{
	None,
	Left,
	Right,

	MAX
};

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	SubmachineGun,

	MAX
};