#include "Task_TraceToTarget.h"

#include <AIController.h>
#include <NavigationSystem.h>
#include <NavigationPath.h>
#include <Blueprint/AIBlueprintHelperLibrary.h>
#include "ShootingPortfolio/Monster/Monster.h"
#include "ShootingPortfolio/Player/PlayerCharacter.h"

EBTNodeResult::Type UTask_TraceToTarget::ExecuteTask(UBehaviorTreeComponent& _OwnerComp, uint8* _NodeMemory)
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

	UAIBlueprintHelperLibrary::SimpleMoveToActor(MonsterController, Target);

	return EBTNodeResult::Succeeded;
}