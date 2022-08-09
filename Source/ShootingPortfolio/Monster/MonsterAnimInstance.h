#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "MonsterAnimInstance.generated.h"

class AMonster;

UCLASS()
class SHOOTINGPORTFOLIO_API UMonsterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
protected:
	UPROPERTY()
	AMonster* m_Monster;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float m_Speed;

public:
	UMonsterAnimInstance();

	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaTime) override;

public:
	UFUNCTION()
	void AnimNotify_RightWeaponAttack();

	UFUNCTION()
	void AnimNotify_RightWeaponAttackEnd();

	UFUNCTION()
	void AnimNotify_LeftWeaponAttack();

	UFUNCTION()
	void AnimNotify_LeftWeaponAttackEnd();
};
