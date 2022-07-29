#include "MonsterAnimInstance.h"

#include "Monster.h"

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
		m_Monster->GetRightWeaponCollsiion()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void UMonsterAnimInstance::AnimNotify_RightWeaponAttackEnd()
{
	m_Monster = m_Monster == nullptr ? Cast<AMonster>(TryGetPawnOwner()) : m_Monster;
	if (m_Monster)
		m_Monster->GetRightWeaponCollsiion()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}