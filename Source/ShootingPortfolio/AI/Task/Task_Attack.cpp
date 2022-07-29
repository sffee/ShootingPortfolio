#include "Task_Attack.h"

#include <AIController.h>
#include "ShootingPortfolio/Monster/Monster.h"

EBTNodeResult::Type UTask_Attack::ExecuteTask(UBehaviorTreeComponent& _OwnerComp, uint8* _NodeMemory)
{
	Super::ExecuteTask(_OwnerComp, _NodeMemory);

	AAIController* MonsterController = _OwnerComp.GetAIOwner();
	if (MonsterController == nullptr)
		return EBTNodeResult::Failed;

	AMonster* Monster = Cast<AMonster>(MonsterController->GetPawn());
	if (Monster == nullptr)
		return EBTNodeResult::Failed;

	if (Monster->PlayAttackIndex(m_AttackIndex) == false)
		return EBTNodeResult::Failed;

	return EBTNodeResult::Succeeded;
}