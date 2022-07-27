#include "PlayerCharacterAnimInstance.h"

#include "PlayerCharacter.h"
#include "ShootingPortfolio/Weapon/Weapon.h"

UPlayerCharacterAnimInstance::UPlayerCharacterAnimInstance()
	: m_Speed(0.f)
	, m_IsAccel(false)
	, m_IsAiming(false)
	, m_YawOffset(0.f)
	, m_Lean(0.f)
	, m_AOYaw(0.f)
	, m_AOPitch(0.f)
	, m_UseAimOffset(true)
	, m_UseRotationRootBone(true)
	, m_UseFABRIK(true)
{

}

void UPlayerCharacterAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
}

void UPlayerCharacterAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);

	m_Player = m_Player == nullptr ? Cast<APlayerCharacter>(TryGetPawnOwner()) : m_Player;
	if (m_Player == nullptr)
		return;

	FVector PlayerVelocity = m_Player->GetVelocity();
	PlayerVelocity.Z = 0.f;
	m_Speed = PlayerVelocity.Size();

	m_IsAccel = 0.f < m_Player->GetCharacterMovement()->GetCurrentAcceleration().Size();
	m_IsAiming = m_Player->IsAiming();
	m_EquipWeapon = m_Player->GetEquipWeapon();
	m_TurnState = m_Player->GetTurnState();
	m_UseRotationRootBone = m_Player->GetUseRotationRootBone();

	CalcYaw(DeltaTime);
	CalcLean(DeltaTime);
	CalcHandTransform(DeltaTime);

	m_AOYaw = m_Player->GetAOYaw();
	m_AOPitch = m_Player->GetAOPitch();

	EPlayerState PlayerState = m_Player->GetState();

	m_UseAimOffset = PlayerState == EPlayerState::Idle || PlayerState == EPlayerState::Fire;
	m_UseFABRIK = PlayerState == EPlayerState::Idle || PlayerState == EPlayerState::Fire;
}

void UPlayerCharacterAnimInstance::CalcYaw(float _DeltaTime)
{
	FRotator AimRotation = m_Player->GetBaseAimRotation();
	FRotator MovementRotation = UKismetMathLibrary::MakeRotFromX(m_Player->GetVelocity());
	FRotator DeltaRot = UKismetMathLibrary::NormalizedDeltaRotator(MovementRotation, AimRotation);
	m_DeltaRotation = FMath::RInterpTo(m_DeltaRotation, DeltaRot, _DeltaTime, 6.f);
	m_YawOffset = m_DeltaRotation.Yaw;
}

void UPlayerCharacterAnimInstance::CalcLean(float _DeltaTime)
{
	m_PlayerRotationPrev = m_PlayerRotation;
	m_PlayerRotation = m_Player->GetActorRotation();
	const FRotator DeltaRot = UKismetMathLibrary::NormalizedDeltaRotator(m_PlayerRotation, m_PlayerRotationPrev);
	const float Target = DeltaRot.Yaw / _DeltaTime;
	const float Interp = FMath::FInterpTo(m_Lean, Target, _DeltaTime, 6.f);
	m_Lean = FMath::Clamp(Interp, -90.f, 90.f);
}

void UPlayerCharacterAnimInstance::CalcHandTransform(float _DeltaTime)
{
	if (m_EquipWeapon == nullptr || m_EquipWeapon->GetMesh() == nullptr || m_Player->GetMesh() == nullptr)
		return;

	m_LeftHandTransform = m_EquipWeapon->GetMesh()->GetSocketTransform(FName("LeftHandSocket"), ERelativeTransformSpace::RTS_World);
	FVector RightHandPosition;
	FRotator RightHandRotation;
	m_Player->GetMesh()->TransformToBoneSpace(FName("hand_r"), m_LeftHandTransform.GetLocation(), FRotator::ZeroRotator, RightHandPosition, RightHandRotation);
	m_LeftHandTransform.SetLocation(RightHandPosition);
	m_LeftHandTransform.SetRotation(FQuat(RightHandRotation));
}

void UPlayerCharacterAnimInstance::AnimNotify_ReloadFinish()
{
	m_Player = m_Player == nullptr ? Cast<APlayerCharacter>(TryGetPawnOwner()) : m_Player;
	if (m_Player)
		m_Player->ReloadFinish();
}