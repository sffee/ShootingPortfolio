#pragma once

#include "Steel.h"

#include "CoreMinimal.h"
#include "../MonsterAnimInstance.h"
#include "SteelAnimInstance.generated.h"

UCLASS()
class SHOOTINGPORTFOLIO_API USteelAnimInstance : public UMonsterAnimInstance
{
	GENERATED_BODY()
	
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	ESteelState m_SteelState;
	
public:
	USteelAnimInstance();

public:
	virtual void NativeUpdateAnimation(float DeltaTime) override;

public:
	UFUNCTION()
	void AnimNotify_Down();

	UFUNCTION()
	void AnimNotify_Idle();
};