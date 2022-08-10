#include "Task_LookAtTarget.h"

#include <AIController.h>
#include "ShootingPortfolio/Monster/Monster.h"

EBTNodeResult::Type UTask_LookAtTarget::ExecuteTask(UBehaviorTreeComponent& _OwnerComp, uint8* _NodeMemory)
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
	FRotator InterpRot = UKismetMathLibrary::RInterpTo(Monster->GetActorRotation(), Rot, GetWorld()->GetDeltaSeconds(), 2.f);
	Monster->SetActorRotation(InterpRot);

	return EBTNodeResult::Succeeded;
}
