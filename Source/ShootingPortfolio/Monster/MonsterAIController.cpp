#include "MonsterAIController.h"

#include "Monster.h"
#include "ShootingPortfolio/Player/PlayerCharacter.h"

AMonsterAIController::AMonsterAIController()
{

}

void AMonsterAIController::BeginPlay()
{
	Super::BeginPlay();

	if (Blackboard)
	{
		APlayerCharacter* Player = Cast<APlayerCharacter>((UGameplayStatics::GetPlayerController(GetWorld(), 0))->GetPawn());
		Blackboard->SetValueAsObject(TEXT("Target"), Player);
	}
}

void AMonsterAIController::OnPossess(APawn* _Monster)
{
	Super::OnPossess(_Monster);

	AMonster* Monster = Cast<AMonster>(_Monster);
	if (Monster)
	{
		m_Blackboard = Monster->GetBlackboard();
		m_BehaviorTree = Monster->GetBehaviorTree();

		if (m_Blackboard)
			UseBlackboard(m_Blackboard, Blackboard);

		if (m_BehaviorTree)
			RunBehaviorTree(m_BehaviorTree);
	}
}