#include "MonsterAnimInstance.h"

#include "Monster.h"
#include <AIController.h>

UMonsterAnimInstance::UMonsterAnimInstance()
	: m_Speed(0.f)
{

}

void UMonsterAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
}

void UMonsterAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);

	m_Monster = m_Monster == nullptr ? Cast<AMonster>(TryGetPawnOwner()) : m_Monster;
	if (m_Monster == nullptr)
		return;

	FVector MonsterVelocity = m_Monster->GetVelocity();
	MonsterVelocity.Z = 0.f;
	m_Speed = MonsterVelocity.Size();
}

void UMonsterAnimInstance::AnimNotify_RightWeaponAttack()
{
	m_Monster = m_Monster == nullptr ? Cast<AMonster>(TryGetPawnOwner()) : m_Monster;
	if (m_Monster)
		m_Monster->RightWeaponCollisionEnable();
}

void UMonsterAnimInstance::AnimNotify_RightWeaponAttackEnd()
{
	m_Monster = m_Monster == nullptr ? Cast<AMonster>(TryGetPawnOwner()) : m_Monster;
	if (m_Monster)
		m_Monster->RightWeaponCollisionDisable();
}

void UMonsterAnimInstance::AnimNotify_LeftWeaponAttack()
{
	m_Monster = m_Monster == nullptr ? Cast<AMonster>(TryGetPawnOwner()) : m_Monster;
	if (m_Monster)
		m_Monster->LeftWeaponCollisionEnable();
}

void UMonsterAnimInstance::AnimNotify_LeftWeaponAttackEnd()
{
	m_Monster = m_Monster == nullptr ? Cast<AMonster>(TryGetPawnOwner()) : m_Monster;
	if (m_Monster)
		m_Monster->LeftWeaponCollisionDisable();
}

void UMonsterAnimInstance::AnimNotify_Movable()
{
	m_Monster = m_Monster == nullptr ? Cast<AMonster>(TryGetPawnOwner()) : m_Monster;
	if (m_Monster)
	{
		AAIController* MosnterController = Cast<AAIController>(m_Monster->GetController());
		if (MosnterController)
		{
			UBlackboardComponent* BlackboardComponent = MosnterController->GetBlackboardComponent();
			if (BlackboardComponent)
				BlackboardComponent->SetValueAsBool(TEXT("Movable"), true);
		}
	}
}

void UMonsterAnimInstance::AnimNotify_MoveDisable()
{
	m_Monster = m_Monster == nullptr ? Cast<AMonster>(TryGetPawnOwner()) : m_Monster;
	if (m_Monster)
	{
		AAIController* MosnterController = Cast<AAIController>(m_Monster->GetController());
		if (MosnterController)
		{
			UBlackboardComponent* BlackboardComponent = MosnterController->GetBlackboardComponent();
			if (BlackboardComponent)
				BlackboardComponent->SetValueAsBool(TEXT("Movable"), false);
		}
	}
}
