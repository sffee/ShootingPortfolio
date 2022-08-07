#include "MonsterAIController.h"

#include "Monster.h"
#include "ShootingPortfolio/Player/PlayerCharacter.h"

AMonsterAIController::AMonsterAIController()
{

}

void AMonsterAIController::BeginPlay()
{
	Super::BeginPlay();
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

		APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		if (PlayerController)
		{
			APlayerCharacter* Player = Cast<APlayerCharacter>(PlayerController->GetPawn());
			if (Player && Blackboard)
			{
				Blackboard->SetValueAsObject(TEXT("Target"), Player);
				Blackboard->SetValueAsBool(TEXT("Movable"), true);
				Blackboard->SetValueAsBool(TEXT("Attackable"), true);
			}
		}
	}
}