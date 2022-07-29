#pragma once

#include "ShootingPortfolio/global.h"

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "Task_Attack.generated.h"

UCLASS()
class SHOOTINGPORTFOLIO_API UTask_Attack : public UBTTaskNode
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	int32 m_AttackIndex;
	
public:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& _OwnerComp, uint8* _NodeMemory) override;
};
