#include "Task_RandomAttack.h"

#include <AIController.h>
#include "ShootingPortfolio/Monster/Monster.h"

EBTNodeResult::Type UTask_RandomAttack::ExecuteTask(UBehaviorTreeComponent& _OwnerComp, uint8* _NodeMemory)
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
	Monster->SetActorRotation(FRotator(0.f, Rot.Yaw, 0.f));

	FRandomStream RandomStream;
	RandomStream.GenerateNewSeed();

	TArray<FName> tmp = m_AttackSectionNames;
	int32 LastIdx = tmp.Num() - 1;
	for (int32 i = 0; i <= LastIdx; i++)
	{
		const int32 Idx = RandomStream.RandRange(i, LastIdx);
		if (i == Idx)
			continue;

		tmp.Swap(i, Idx);
	}

	EBTNodeResult::Type Result = EBTNodeResult::Failed;

	for (int32 i = 0; i < tmp.Num(); i++)
	{
		if (Monster->PlayAttackSection(tmp[i]))
		{
			MonsterController->StopMovement();
			bNotifyTick = true;

			Result = EBTNodeResult::InProgress;
			break;
		}
	}

	return Result;
}

void UTask_RandomAttack::TickTask(UBehaviorTreeComponent& _OwnerComp, uint8* _NodeMemory, float _DeltaSeconds)
{
	Super::TickTask(_OwnerComp, _NodeMemory, _DeltaSeconds);

	AAIController* MonsterController = _OwnerComp.GetAIOwner();
	if (MonsterController == nullptr)
		return;

	AMonster* Monster = Cast<AMonster>(MonsterController->GetPawn());
	if (Monster == nullptr)
		return;

	if (Monster->IsAttacking())
		return;

	return FinishLatentTask(_OwnerComp, EBTNodeResult::Succeeded);
}