#pragma once

#include "ShootingPortfolio/global.h"

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "Task_RandomAttack.generated.h"

UCLASS()
class SHOOTINGPORTFOLIO_API UTask_RandomAttack : public UBTTaskNode
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	TArray<FName> m_AttackSectionNames;

public:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& _OwnerComp, uint8* _NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& _OwnerComp, uint8* _NodeMemory, float _DeltaSeconds) override;
};