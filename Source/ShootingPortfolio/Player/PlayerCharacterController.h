#pragma once

#include "ShootingPortfolio/global.h"
#include "ShootingPortfolio/StructData.h"

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PlayerCharacterController.generated.h"

class APlayerCharacter;
class AShootingHUD;
class AWeapon;

UCLASS()
class SHOOTINGPORTFOLIO_API APlayerCharacterController : public APlayerController
{
	GENERATED_BODY()
	
	friend APlayerCharacter;
	
private:
	APlayerCharacter* m_Player;
	AWeapon* m_EquipWeapon;
	AShootingHUD* m_HUD;

private:
	UPROPERTY(VisibleAnywhere, Category = "Wave")
	EWaveState m_WaveState;
	
	UPROPERTY(EditAnywhere, Category = "Wave")
	float m_StartWaveCountTime;

	UPROPERTY(VisibleAnywhere, Category = "Wave")
	float m_RemainingWaveCountTime;
	int32 m_PrevReminingWaveCountTime;

private:
	UPROPERTY(EditAnywhere, Category = "Player | Status")
	FPlayerStatus m_Status;

	UPROPERTY(VisibleAnywhere, Category = "Player | Ammo")
	TMap<EWeaponType, int32> m_AmmoMap;

	UPROPERTY(EditAnywhere, Category = "Player | Ammo")
	FStartAmmo m_StartAmmo;

	bool m_FirstHUDUpdateComplete;

	FHitResult m_TraceHitResult;

private:
	FTimerHandle m_WaveTimer;

private:
	APlayerCharacterController();

public:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

public:
	void SetWaveState(EWaveState _State);

private:
	void WaveCountdown();
	void WaveStart();
	void WaveStartTimerEnd();
	void WavePlay();
	void WaveComplete();
	void WaveCompleteTimerEnd();

private:
	void UpdateWaveCountdown(float _DeltaTime);

private:
	void InitAmmo();
	void UpdateFirstHUD();

public:
	void AddHP(float _Value);
	void AddStamina(float _Value);
	void SubAmmo();
	void ReloadFinish();
	bool AmmoMapEmpty(EWeaponType _Type);

public:
	void UpdateHPHUD();
	void UpdateStaminaHUD();
	void UpdateAmmoHUD();
};
