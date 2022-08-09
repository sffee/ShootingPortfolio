#pragma once

#include "CoreMinimal.h"
#include "../MonsterAnimInstance.h"
#include "KhaimeraAnimInstance.generated.h"

UCLASS()
class SHOOTINGPORTFOLIO_API UKhaimeraAnimInstance : public UMonsterAnimInstance
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere)
	FVector m_JumpVelocity;

	UPROPERTY(EditAnywhere)
	float m_JumpAttackGravity;

	UPROPERTY(EditAnywhere)
	float m_JumpAttackAnimPauseGravity;

	FVector m_DownAttackLocation;

public:
	UKhaimeraAnimInstance();

private:
	void DownAttack();

public:
	UFUNCTION()
	void AnimNotify_Jump();

	UFUNCTION()
	void AnimNotify_AnimPause();

	UFUNCTION()
	void AnimNotify_DestroyWarningMark();
};
