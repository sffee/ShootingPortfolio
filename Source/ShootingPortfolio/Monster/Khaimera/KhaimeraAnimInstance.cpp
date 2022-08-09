#include "KhaimeraAnimInstance.h"

#include "ShootingPortfolio/Player/PlayerCharacter.h"
#include "ShootingPortfolio/Monster/Monster.h"
#include "Khaimera.h"

UKhaimeraAnimInstance::UKhaimeraAnimInstance()
	: m_JumpVelocity(FVector(100.f, 100.f, 1500.f))
	, m_JumpAttackGravity(3.5f)
	, m_JumpAttackAnimPauseGravity(1.f)
{

}

void UKhaimeraAnimInstance::DownAttack()
{
	m_Monster = m_Monster == nullptr ? Cast<AMonster>(TryGetPawnOwner()) : m_Monster;
	if (m_Monster)
	{
		m_Monster->GetCharacterMovement()->GravityScale = 1.f;
		m_Monster->GetMesh()->bPauseAnims = false;

		FLatentActionInfo Info;
		Info.CallbackTarget = this;
		UKismetSystemLibrary::MoveComponentTo(m_Monster->GetRootComponent(), m_DownAttackLocation, m_Monster->GetRootComponent()->GetRelativeRotation(), true, true, 0.4f, false, EMoveComponentAction::Type::Move, Info);
	}
}

void UKhaimeraAnimInstance::AnimNotify_Jump()
{
	AKhaimera* Monster = Cast<AKhaimera>(TryGetPawnOwner());
	AActor* Player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	if (m_Monster || Player)
	{
		m_DownAttackLocation = Player->GetActorLocation() + (Player->GetActorForwardVector() * 200.f);
		
		Monster->GetCharacterMovement()->GravityScale = m_JumpAttackGravity;
		Monster->GetController()->StopMovement();

		Monster->CreateWarningMark(Player->GetActorLocation());

		FVector JumpVelocity = Monster->GetActorForwardVector() * m_JumpVelocity;
		JumpVelocity.Z = m_JumpVelocity.Z;
		Monster->LaunchCharacter(JumpVelocity, false, false);
	}
}

void UKhaimeraAnimInstance::AnimNotify_AnimPause()
{
	m_Monster = m_Monster == nullptr ? Cast<AMonster>(TryGetPawnOwner()) : m_Monster;
	if (m_Monster)
	{
		m_Monster->GetCharacterMovement()->GravityScale = m_JumpAttackAnimPauseGravity;
		m_Monster->GetMesh()->bPauseAnims = true;

		FTimerHandle Timer;
		m_Monster->GetWorldTimerManager().SetTimer(Timer, this, &UKhaimeraAnimInstance::DownAttack, 0.5f);
	}
}

void UKhaimeraAnimInstance::AnimNotify_DestroyWarningMark()
{
	AKhaimera* Monster = Cast<AKhaimera>(TryGetPawnOwner());
	if (Monster)
	{
		Monster->DestroyWarningMark();
	}
}