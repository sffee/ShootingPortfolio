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

	UBlackboardComponent* MonsterBlackboardComponent = MonsterController->GetBlackboardComponent();
	if (MonsterBlackboardComponent == nullptr)
		return EBTNodeResult::Failed;

	AActor* Target = Cast<AActor>(MonsterBlackboardComponent->GetValueAsObject(TEXT("Target")));
	if (Target == nullptr)
		return EBTNodeResult::Failed;

	FRotator Rot = UKismetMathLibrary::FindLookAtRotation(Monster->GetActorLocation(), Target->GetActorLocation());
	Monster->SetActorRotation(Rot);

	if (Monster->PlayAttackSection(m_AttackSectionName) == false)
		return EBTNodeResult::Failed;

	MonsterController->StopMovement();

	return EBTNodeResult::Succeeded;
}