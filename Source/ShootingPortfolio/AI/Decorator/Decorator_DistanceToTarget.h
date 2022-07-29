#pragma once

#include "ShootingPortfolio/global.h"

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "Decorator_DistanceToTarget.generated.h"

UCLASS()
class SHOOTINGPORTFOLIO_API UDecorator_DistanceToTarget : public UBTDecorator
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditAnywhere, Category = Condition, meta = (AllowPrivateAccess = "true"))
	float m_CheckDistance;

public:
	UDecorator_DistanceToTarget();

public:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& _OwnerComp, uint8* _NodeMemory) const override;
};
