#pragma once

#include "ShootingPortfolio/global.h"

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "Task_LookAtTarget.generated.h"

UCLASS()
class SHOOTINGPORTFOLIO_API UTask_LookAtTarget : public UBTTaskNode
{
	GENERATED_BODY()

public:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& _OwnerComp, uint8* _NodeMemory) override;
};
