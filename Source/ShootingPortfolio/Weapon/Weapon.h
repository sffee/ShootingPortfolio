#pragma once

#include "ShootingPortfolio/global.h"

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapon.generated.h"

class UAnimationAsset;

UCLASS()
class SHOOTINGPORTFOLIO_API AWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	AWeapon();

private:
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* m_Mesh;

protected:
	UPROPERTY(EditAnywhere, Category = Weapon)
	FString m_Name;

	UPROPERTY(EditAnywhere, Category = Weapon)
	UTexture2D* m_WeaponIcon;
	
	UPROPERTY(EditAnywhere, Category = Weapon)
	EWeaponType m_Type;

	UPROPERTY(EditAnywhere, Category = Weapon)
	FString m_AttachSocketName;

	UPROPERTY(EditAnywhere, Category = Weapon)
	float m_IdleFireBlendWeight;

	UPROPERTY(EditAnywhere, Category = Weapon)
	float m_AimingFireBlendWeight;

	UPROPERTY(VisibleAnywhere, Category = Weapon)
	int32 m_InventorySlotIndex;

	UPROPERTY(EditAnywhere, Category = Weapon)
	int32 m_Damage;

	UPROPERTY(EditAnywhere, Category = Weapon)
	float m_HeadDamageRate;

	UPROPERTY(EditAnywhere, Category = Weapon)
	float m_Range;

protected:
	UPROPERTY(EditAnywhere, Category = Ammo)
	int32 m_Ammo;

	UPROPERTY(EditAnywhere, Category = Ammo)
	int32 m_Magazine;

	UPROPERTY(EditAnywhere, Category = Fire)
	bool m_InfinityMagazine;

protected:
	UPROPERTY(EditAnywhere, Category = Crosshair)
	ECrosshairType m_CrosshairType;

	UPROPERTY(EditAnywhere, Category = Crosshair)
	float m_CrosshairRecoil;

protected:
	UPROPERTY(EditAnywhere, Category = Zoom)
	float m_CameraZoomFOV;

	UPROPERTY(EditAnywhere, Category = Zoom)
	float m_CameraZoomSpeed;

protected:
	UPROPERTY(EditAnywhere, Category = Fire)
	USoundCue* m_FireSound;

	UPROPERTY(EditAnywhere, Category = Fire)
	UParticleSystem* m_MuzzleFlashParticle;

	UPROPERTY(EditAnywhere, Category = Fire)
	UParticleSystem* m_HitParticle;

	UPROPERTY(EditAnywhere, Category = Fire)
	TSubclassOf<UCameraShakeBase> m_FireCameraShake;

	UPROPERTY(EditAnywhere, Category = Fire)
	UAnimationAsset* m_FireAnimation;
	
	UPROPERTY(EditAnywhere, Category = Fire)
	bool m_IsAutoFire;

	UPROPERTY(EditAnywhere, Category = Fire, meta = (EditCondition = "m_IsAutoFire"))
	float m_FireDelay;

	UPROPERTY(VisibleAnywhere, Category = Fire)
	bool m_CanFire;

	FTimerHandle m_CanFireTimer;

protected:
	UPROPERTY(EditAnywhere, Category = Reload)
	FName m_ReloadSectionName;

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

public:
	virtual void Fire(const FHitResult& _TraceHitResult);
	void SetAmmo(int32 _Ammo);
	void AddAmmo(int32 _Ammo);
	void SpawnDamageText(const FHitResult& _HitResult, float _Damage, bool _IsHeadShot);

protected:
	void PlaySound(USoundCue* _Sound);
	void PlayMuzzleFlashParticle();
	void PlayCameraShake();
	void PlayFireAnimation();

protected:
	void PlayHitParticle(const FVector& _Location);

private:
	void CanFireTimerEnd();

public:
	FORCEINLINE USkeletalMeshComponent* GetMesh() const { return m_Mesh; }
	FORCEINLINE ECrosshairType GetCrosshairType() const { return m_CrosshairType; }
	FORCEINLINE float GetCrosshairRecoil() const { return m_CrosshairRecoil; }
	FORCEINLINE float GetCameraZoomFOV() const { return m_CameraZoomFOV; }
	FORCEINLINE float GetCameraZoomSpeed() const { return m_CameraZoomSpeed; }
	FORCEINLINE bool IsAutoFire() const { return m_IsAutoFire; }
	FORCEINLINE float GetFireDelay() const { return m_FireDelay; }
	FORCEINLINE FName GetReloadSectionName() const { return m_ReloadSectionName; }
	FORCEINLINE EWeaponType GetWeaponType() const { return m_Type; }
	FORCEINLINE FString GetWeaponName() const { return m_Name; }
	FORCEINLINE int32 GetAmmo() const { return m_Ammo; }
	FORCEINLINE int32 GetMagazine() const { return m_Magazine; }
	FORCEINLINE float GetRange() const { return m_Range; }
	FORCEINLINE UTexture2D* GetWeaponIcon() const { return m_WeaponIcon; }
	FORCEINLINE int32 GetInventorySlotIndex() const { return m_InventorySlotIndex; }
	FORCEINLINE FString GetAttachSocketName() const { return m_AttachSocketName; }
	FORCEINLINE float GetIdleFireBlendWeight() const { return m_IdleFireBlendWeight; }
	FORCEINLINE float GetAimingFireBlendWeight() const { return m_AimingFireBlendWeight; }
	FORCEINLINE bool GetInfinityMagazine() const { return m_InfinityMagazine; }

public:
	FORCEINLINE bool AmmoEmpty() const { return m_Ammo == 0; }
	FORCEINLINE bool AmmoFull() const { return m_Magazine <= m_Ammo; }
	FORCEINLINE void SetInventorySlotIndex(int32 _InventorySlotIndex) { m_InventorySlotIndex = _InventorySlotIndex; }
};
