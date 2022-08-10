#pragma once

#include "CoreMinimal.h"
#include "../MonsterAnimInstance.h"
#include "KhaimeraAnimInstance.generated.h"

class APlayerCharacter;
class ASpawnMonsterCircle;

UCLASS()
class SHOOTINGPORTFOLIO_API UKhaimeraAnimInstance : public UMonsterAnimInstance
{
	GENERATED_BODY()
	
private:
	UPROPERTY()
	APlayerCharacter* m_Player;
	
private:
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	FVector m_JumpVelocity;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	float m_JumpAttackGravity;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	float m_JumpAttackAnimPauseGravity;

	FVector m_DownAttackLocation;

	bool m_IsLookAtPlayer;

	TArray<ASpawnMonsterCircle*> m_SpawnMonsterCircles;

public:
	UKhaimeraAnimInstance();

public:
	virtual void NativeUpdateAnimation(float DeltaTime) override;

private:
	void DownAttack();
	void LookAtPlayer(float _DeltaTime);

public:
	UFUNCTION()
	void AnimNotify_Jump();

	UFUNCTION()
	void AnimNotify_AnimPause();

	UFUNCTION()
	void AnimNotify_DestroyWarningMark();

	UFUNCTION()
	void AnimNotify_Dash();

	UFUNCTION()
	void AnimNotify_LookAtPlayer();

	UFUNCTION()
	void AnimNotify_LookAtPlayerEnd();

	UFUNCTION()
	void AnimNotify_SpawnCircle();

	UFUNCTION()
	void AnimNotify_SpawnMonsterHowl();
};
