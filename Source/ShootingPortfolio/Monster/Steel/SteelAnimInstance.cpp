#include "SteelAnimInstance.h"

#include <AIController.h>

USteelAnimInstance::USteelAnimInstance()
{

}

void USteelAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);

	ASteel* Steel = Cast<ASteel>(TryGetPawnOwner());
	if (Steel)
		m_SteelState = Steel->GetSteelState();
}

void USteelAnimInstance::AnimNotify_Down()
{
	ASteel* Steel = Cast<ASteel>(TryGetPawnOwner());
	if (Steel)
	{
		Steel->PlayMontage(Steel->GetShieldBreakMontage(), TEXT("Down"));

		AAIController* Controller = Cast<AAIController>(Steel->GetController());
		if (Controller)
		{
			Controller->StopMovement();

			UBlackboardComponent* BlackboardComponent = Controller->GetBlackboardComponent();
			if (BlackboardComponent)
			{
				BlackboardComponent->SetValueAsBool(TEXT("Attackable"), false);
				BlackboardComponent->SetValueAsBool(TEXT("Movable"), false);
				BlackboardComponent->SetValueAsBool(TEXT("EquipShield"), false);
			}
		}
	}
}

void USteelAnimInstance::AnimNotify_Idle()
{
	m_SteelState = ESteelState::Idle;

	ASteel* Steel = Cast<ASteel>(TryGetPawnOwner());
	if (Steel)
	{
		Steel->SetSteelState(ESteelState::Idle);

		AAIController* Controller = Cast<AAIController>(Steel->GetController());
		if (Controller)
		{
			UBlackboardComponent* BlackboardComponent = Controller->GetBlackboardComponent();
			if (BlackboardComponent)
			{
				BlackboardComponent->SetValueAsBool(TEXT("Attackable"), true);
				BlackboardComponent->SetValueAsBool(TEXT("Movable"), true);
			}
		}
	}
}