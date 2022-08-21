#include "KhaimeraAnimInstance.h"

#include "ShootingPortfolio/Player/PlayerCharacter.h"
#include "ShootingPortfolio/Monster/Monster.h"
#include "Khaimera.h"
#include "SpawnMonster/SpawnMonsterCircle.h"
#include <AIController.h>

UKhaimeraAnimInstance::UKhaimeraAnimInstance()
	: m_JumpVelocity(FVector(100.f, 100.f, 1500.f))
	, m_JumpAttackGravity(3.5f)
	, m_JumpAttackAnimPauseGravity(1.f)
	, m_IsLookAtPlayer(false)
{

}

void UKhaimeraAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);

	LookAtPlayer(DeltaTime);
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

void UKhaimeraAnimInstance::LookAtPlayer(float _DeltaTime)
{
	if (m_IsLookAtPlayer == false)
		return;

	m_Monster = m_Monster == nullptr ? Cast<AMonster>(TryGetPawnOwner()) : m_Monster;
	m_Player = m_Player == nullptr ? Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)) : m_Player;
	if (m_Monster && m_Player)
	{
		FRotator Rot = UKismetMathLibrary::FindLookAtRotation(m_Monster->GetActorLocation(), m_Player->GetActorLocation());
		float InterpYaw = UKismetMathLibrary::FInterpTo(m_Monster->GetActorRotation().Yaw, Rot.Yaw, _DeltaTime, 2.f);

		m_Monster->SetActorRotation(FRotator(0.f, InterpYaw, 0.f));
	}
}

void UKhaimeraAnimInstance::AnimNotify_Jump()
{
	AKhaimera* Monster = Cast<AKhaimera>(TryGetPawnOwner());
	m_Player = m_Player == nullptr ? Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)) : m_Player;
	if (Monster && m_Player)
	{
		float CapsuleSize = Monster->GetCapsuleComponent()->GetScaledCapsuleHalfHeight() - m_Player->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
		m_DownAttackLocation = m_Player->GetActorLocation() + (m_Player->GetActorForwardVector() * 200.f) + FVector(0.f, 0.f, CapsuleSize);

		Monster->GetCharacterMovement()->GravityScale = m_JumpAttackGravity;
		Monster->GetController()->StopMovement();

		Monster->CreateWarningMark(m_Player->GetActorLocation());

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

void UKhaimeraAnimInstance::AnimNotify_Dash()
{
	m_Monster = m_Monster == nullptr ? Cast<AMonster>(TryGetPawnOwner()) : m_Monster;
	m_Player = m_Player == nullptr ? Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)) : m_Player;
	if (m_Monster && m_Player)
	{
		float CapsuleSize = m_Monster->GetCapsuleComponent()->GetScaledCapsuleHalfHeight() - m_Player->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
		FVector DashLocation = m_Player->GetActorLocation() + (m_Player->GetActorForwardVector() * 150.f) + FVector(0.f, 0.f, CapsuleSize);

		FLatentActionInfo Info;
		Info.CallbackTarget = this;
		UKismetSystemLibrary::MoveComponentTo(m_Monster->GetRootComponent(), DashLocation, m_Monster->GetRootComponent()->GetRelativeRotation(), true, true, 0.3f, false, EMoveComponentAction::Type::Move, Info);
	}
}

void UKhaimeraAnimInstance::AnimNotify_LookAtPlayer()
{
	m_IsLookAtPlayer = true;
}

void UKhaimeraAnimInstance::AnimNotify_LookAtPlayerEnd()
{
	m_IsLookAtPlayer = false;
}

void UKhaimeraAnimInstance::AnimNotify_SpawnCircle()
{
	AKhaimera* Monster = Cast<AKhaimera>(TryGetPawnOwner());
	if (Monster)
	{
		m_SpawnMonsterCircles.Empty();

		const FVector MonsterLocation = Monster->GetActorLocation();
		const FVector MonsterForwardVector = Monster->GetActorForwardVector();

		FActorSpawnParameters Param;
		Param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		for (int32 i = 0; i < 3; i++)
		{
			float Angle = -30.f + (i * 30.f);
			
			FVector Start = MonsterForwardVector.RotateAngleAxis(Angle, FVector(0.f, 0.f, 1.f)) * 800.f;
			Start += MonsterLocation;
			Start.Z = MonsterLocation.Z + 200.f;

			FVector End = Start + FVector(0.f, 0.f, -1000.f);

			FHitResult HitResult;
			GetWorld()->LineTraceSingleByProfile(HitResult, Start, End, FName("LandScape"));

			if (HitResult.bBlockingHit == false)
				return;

			ASpawnMonsterCircle* SpawnMonsterCircle = GetWorld()->SpawnActor<ASpawnMonsterCircle>(Monster->GetSpawnMonsterCircleClass(), HitResult.ImpactPoint, m_Monster->GetActorRotation());
			m_SpawnMonsterCircles.Add(SpawnMonsterCircle);
		}
	}
}

void UKhaimeraAnimInstance::AnimNotify_SpawnMonster()
{
	m_Monster = m_Monster == nullptr ? Cast<AMonster>(TryGetPawnOwner()) : m_Monster;
	if (m_Monster)
	{
		for (auto& Circle : m_SpawnMonsterCircles)
			Circle->MonsterSpawn();
	}
}

void UKhaimeraAnimInstance::AnimNotify_JumpAttack()
{
	AKhaimera* Monster = Cast<AKhaimera>(TryGetPawnOwner());
	if (Monster)
		Monster->JumpAttack();
}