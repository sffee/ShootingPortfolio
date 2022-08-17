#pragma once

#include "global.h"

#include "CoreMinimal.h"
#include "StructData.generated.h"

USTRUCT(BlueprintType)
struct FCrosshairTexture : public FTableRowBase
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere)
	UTexture2D* CenterTexture;

	UPROPERTY(EditAnywhere)
	UTexture2D* TopTexture;

	UPROPERTY(EditAnywhere)
	UTexture2D* BottomTexture;

	UPROPERTY(EditAnywhere)
	UTexture2D* LeftTexture;

	UPROPERTY(EditAnywhere)
	UTexture2D* RightTexture;
};

USTRUCT(BlueprintType)
struct FPlayerStatus
{
	GENERATED_BODY()
	
	UPROPERTY(VisibleAnywhere)
	float CurHP;
	
	UPROPERTY(EditAnywhere)
	float MaxHP;

	UPROPERTY(VisibleAnywhere)
	float CurStamina;

	UPROPERTY(EditAnywhere)
	float MaxStamina;

	FPlayerStatus()
		: CurHP(0.f)
		, MaxHP(200.f)
		, CurStamina(0.f)
		, MaxStamina(100.f)
	{
	}
};

USTRUCT(BlueprintType)
struct FMonsterStatus
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere)
	float CurHP;

	UPROPERTY(EditAnywhere)
	float MaxHP;

	UPROPERTY(VisibleAnywhere)
	float CurShield;

	UPROPERTY(EditAnywhere)
	float MaxShield;

	FMonsterStatus()
		: CurHP(0.f)
		, MaxHP(0.f)
		, CurShield(0.f)
		, MaxShield(0.f)
	{
	}
};

USTRUCT(BlueprintType)
struct FMonsterSpawnData
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<class AMonster> SpawnMonster;

	UPROPERTY(EditAnywhere)
	int32 SpawnCount;

	UPROPERTY(EditAnywhere)
	float SpawnDelay;
};

USTRUCT(BlueprintType)
struct FMonsterWave : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	TArray<FMonsterSpawnData> MonsterSpawnData;
};

USTRUCT(BlueprintType)
struct FMonsterAttackInfo : public FTableRowBase
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere)
	float Damage;

	UPROPERTY(EditAnywhere)
	float Cooltime;
};

USTRUCT(BlueprintType)
struct FMonsterBuffInfo : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	float Cooltime;
};

USTRUCT(BlueprintType)
struct FAmmoData
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere)
	int32 CurAmmo;

	UPROPERTY(EditAnywhere)
	int32 MaxAmmo;

	FAmmoData()
	{
	}

	FAmmoData(int32 _CurAmmo, int32 _MaxAmmo)
		: CurAmmo(_CurAmmo), MaxAmmo(_MaxAmmo)
	{
	}
};

USTRUCT(BlueprintType)
struct FStartAmmo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	FAmmoData SubmachineGun;

	UPROPERTY(EditAnywhere)
	FAmmoData RocketLauncher;

	UPROPERTY(EditAnywhere)
	FAmmoData SniperRifle;

	FStartAmmo()
		: SubmachineGun({ 30, 600 })
		, RocketLauncher({ 3, 18 })
		, SniperRifle({ 5, 30 })
	{
	}
};