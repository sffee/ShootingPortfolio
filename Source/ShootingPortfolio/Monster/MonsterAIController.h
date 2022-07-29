#pragma once

#include "ShootingPortfolio/global.h"

#include "CoreMinimal.h"
#include "AIController.h"
#include "MonsterAIController.generated.h"

UCLASS()
class SHOOTINGPORTFOLIO_API AMonsterAIController : public AAIController
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditAnywhere, Category = AI)
	UBehaviorTree* m_BehaviorTree;

	UPROPERTY(EditAnywhere, Category = AI)
	UBlackboardData* m_Blackboard;

private:
	AMonsterAIController();

	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* _Monster);
};