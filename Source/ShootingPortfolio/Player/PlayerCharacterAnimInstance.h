#pragma once

#include "ShootingPortfolio/global.h"

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "PlayerCharacterAnimInstance.generated.h"

class APlayerCharacter;
class AWeapon;

UCLASS()
class SHOOTINGPORTFOLIO_API UPlayerCharacterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
private:
	UPROPERTY()
	APlayerCharacter* m_Player;
	
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float m_Speed;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	bool m_IsAccel;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	bool m_IsAiming;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	AWeapon* m_EquipWeapon;

private:
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float m_YawOffset;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float m_Lean;

	FRotator m_PlayerRotationPrev;
	FRotator m_PlayerRotation;
	FRotator m_DeltaRotation;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	ETurnState m_TurnState;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float m_AOYaw;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float m_AOPitch;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	bool m_UseAimOffset;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	bool m_UseRotationRootBone;

private:
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	FTransform m_LeftHandTransform;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	bool m_UseFABRIK;

public:
	UPlayerCharacterAnimInstance();

	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaTime) override;

private:
	void CalcYaw(float _DeltaTime);
	void CalcLean(float _DeltaTime);
	void CalcHandTransform(float _DeltaTime);

public:
	UFUNCTION()
	void AnimNotify_ReloadFinish();

	UFUNCTION()
	void AnimNotify_EquipFinish();
};
