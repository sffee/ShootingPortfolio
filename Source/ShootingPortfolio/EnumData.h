#pragma once

UENUM(BlueprintType)
enum class ECrosshairType : uint8
{
	Cross,
	Circle,

	MAX
};

UENUM(BlueprintType)
enum class EPlayerState : uint8
{
	Idle,
	Sprint,
	RollDive,
	Reloading,
	Equipping,

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
	Pistol,
	SubmachineGun,
	RocketLauncher,

	MAX
};

UENUM(BlueprintType)
enum class EWaveState : uint8
{
	Countdown,
	Start,
	Play,
	Complete,

	MAX
};