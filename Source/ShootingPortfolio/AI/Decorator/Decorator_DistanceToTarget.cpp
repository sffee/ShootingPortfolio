#include "Decorator_DistanceToTarget.h"

#include <BehaviorTree/BlackboardComponent.h>
#include <AIController.h>

UDecorator_DistanceToTarget::UDecorator_DistanceToTarget()
	: m_MinDistance(0.f)
	, m_MaxDistance(0.f)
{

}

bool UDecorator_DistanceToTarget::CalculateRawConditionValue(UBehaviorTreeComponent& _OwnerComp, uint8* _NodeMemory) const
{	
	AAIController* MonsterController = _OwnerComp.GetAIOwner();
	if (MonsterController == nullptr)
		return false;

	APawn* Monster = Cast<APawn>(MonsterController->GetPawn());
	if (Monster == nullptr)
		return false;

	UBlackboardComponent* BB = MonsterController->GetBlackboardComponent();
	if (BB == nullptr)
		return false;

	APawn* Target = Cast<APawn>(BB->GetValueAsObject(TEXT("Target")));
	if (Target == nullptr)
		return false;

	float Distance = FVector::Distance(Target->GetActorLocation(), Monster->GetActorLocation());
	if (m_MinDistance <= Distance && Distance <= m_MaxDistance)
		return true;

	return false;
}