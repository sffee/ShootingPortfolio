#pragma once

#include "ShootingPortfolio/global.h"

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlayerCharacter.generated.h"

class AWeapon;
class APlayerCharacterController;
class AShootingHUD;

UCLASS()
class SHOOTINGPORTFOLIO_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()
	
private:
	APlayerCharacterController* m_Controller;
	AShootingHUD* m_HUD;
	
private:
	UPROPERTY(EditAnywhere, Category = Movement)
	float m_TurnRate;

	UPROPERTY(EditAnywhere, Category = Movement)
	float m_AimingTurnRate;

	UPROPERTY(EditAnywhere, Category = Movement)
	float m_LookUpRate;

	UPROPERTY(EditAnywhere, Category = Movement)
	float m_AimingLookUpRate;

	UPROPERTY(VisibleAnywhere, Category = Movement)
	float m_DefaultMoveSpeed;

	UPROPERTY(VisibleAnywhere, Category = Movement)
	float m_AimingMoveSpeed;

	UPROPERTY()
	USpringArmComponent* m_CameraArm;

	UPROPERTY()
	UCameraComponent* m_FollowCamera;

private:
	UPROPERTY(VisibleAnywhere, Category = Info)
	EPlayerState m_State;
	
	UPROPERTY(VisibleAnywhere, Category = Info)
	float m_DefaultCameraFOV;

	UPROPERTY(VisibleAnywhere, Category = Info)
	float m_CurCameraFOV;

	UPROPERTY(EditAnywhere, Category = Info)
	float m_ZoomReleaseSpeed;

private:
	UPROPERTY(EditAnywhere, Category = Weapon, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AWeapon> m_DefaultWeapon;

	UPROPERTY(VisibleAnywhere, Category = Weapon)
	AWeapon* m_EquipWeapon;

	UPROPERTY(VisibleAnywhere, Category = Weapon)
	UAnimMontage* m_FireAnimMontage;

	UPROPERTY(VisibleAnywhere, Category = Weapon)
	UAnimMontage* m_ReloadAnimMontage;
	
	FHitResult m_TraceHitResult;

	FTimerHandle m_AutoFireTimer;

private:
	bool m_AimingButton;
	bool m_IsAiming;

	bool m_FireButtonPress;

private:
	float m_CrosshairAimingValue;
	float m_CrosshairRecoilValue;
	float m_CrosshairSpreadMultiple;

private:
	ETurnState m_TurnState;
	
	float m_AOYaw;
	float m_AOPitch;
	FRotator m_StartAimRotation;

	bool m_UseRotationRootBone;

public:
	APlayerCharacter();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	void MoveForward(float _Value);
	void MoveRight(float _Value);
	void Turn(float _Value);
	void LookUp(float _Value);

	void FireButtonPressed();
	void FireButtonReleased();

	void AimingButtonPressed();
	void AimingButtonReleased();

	void ReloadPressed();

private:
	AWeapon* SpawnDefaultWeapon();
	void EquipWeapon(AWeapon* _Weapon);

	void Aiming();
	void StopAiming();

	void Fire();
	void FireTimerEnd();

	void Reloading();

public:
	void ReloadFinish();

private:
	void UpdateCameraFOV(float _DeltaTime);
	void UpdateTraceHitResult();
	void UpdateCrosshairHUD(float _DeltaTime);
	void UpdateAimOffset(float _DeltaTime);

private:
	void TurnInPlace(float _DeltaTime);

public:
	void PlayMontage(UAnimMontage* _AnimMontage, FName _SectionName);

public:
	FORCEINLINE void SetState(EPlayerState _State) { m_State = _State; }

public:
	FORCEINLINE bool IsAiming() const { return m_IsAiming; }
	FORCEINLINE UAnimMontage* GetFireAnimMontage() const { return m_FireAnimMontage; }
	FORCEINLINE const FHitResult& GetTraceHitResult() const { return m_TraceHitResult; }
	FORCEINLINE EPlayerState GetState() const { return m_State; }
	FORCEINLINE AWeapon* GetEquipWeapon() const { return m_EquipWeapon; }
	FORCEINLINE ETurnState GetTurnState() const { return m_TurnState; }
	FORCEINLINE bool GetUseRotationRootBone() const { return m_UseRotationRootBone; }
	FORCEINLINE float GetAOYaw() const { return m_AOYaw; }
	FORCEINLINE float GetAOPitch() const { return m_AOPitch; }
};
