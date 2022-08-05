#include "PlayerCharacter.h"

#include "PlayerCharacterController.h"
#include "ShootingPortfolio/Weapon/Weapon.h"
#include "ShootingPortfolio/UI/ShootingHUD.h"
#include "ShootingPortfolio/Monster/Monster.h"

APlayerCharacter::APlayerCharacter()
	: m_TurnRate(45.f)
	, m_AimingTurnRate(20.f)
	, m_LookUpRate(45.f)
	, m_AimingLookUpRate(20.f)
	, m_DefaultMoveSpeed(600.f)
	, m_AimingMoveSpeed(300.f)
	, m_SprintMoveSpeed(900.f)
	, m_State(EPlayerState::Idle)
	, m_SprintStaminaConsumeSpeed(20.f)
	, m_StaminaRestoreSpeed(10.f)
	, m_StaminaRestoreStartTime(1.5f)
	, m_StaminaRestore(true)
	, m_DefaultCameraFOV(0.f)
	, m_CurCameraFOV(0.f)
	, m_SprintCameraFOV(60.f)
	, m_SprintCameraZoomSpeed(5.f)
	, m_ZoomReleaseSpeed(20.f)
	, m_CanDamage(true)
	, m_CanDamageTimeRate(0.8f)
	, m_IsMoveForward(false)
	, m_IsMoveSide(false)
	, m_AimingButton(false)
	, m_IsAiming(false)
	, m_FireButtonPress(false)
	, m_SprintButtonPress(false)
	, m_IsSprint(false)
	, m_CrosshairAimingValue(0.f)
	, m_CrosshairRecoilValue(0.f)
	, m_CrosshairRecoilMaxValue(3.75f)
	, m_CrosshairSpreadMultiple(16.f)
	, m_CrosshairSpread(0.f)
	, m_TurnState(ETurnState::None)
	, m_AOYaw(0.f)
	, m_AOPitch(0.f)
	, m_UseRotationRootBone(true)
{
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> PlayerMesh(TEXT("SkeletalMesh'/Game/ParagonLtBelica/Characters/Heroes/Belica/Meshes/Belica.Belica'"));
	if (PlayerMesh.Succeeded())
		GetMesh()->SetSkeletalMesh(PlayerMesh.Object);

	GetMesh()->SetWorldLocation(FVector(0.f, 0.f, -88.f));
	GetMesh()->SetWorldRotation(FRotator(0.f, -90.f, 0.f));

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->RotationRate = FRotator(0.f, 540.f, 0.f);
	GetCharacterMovement()->bOrientRotationToMovement = false;

	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Player"));

	m_CameraArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	m_CameraArm->SetupAttachment(RootComponent);
	m_CameraArm->TargetArmLength = 300.f;
	m_CameraArm->bUsePawnControlRotation = true;
	m_CameraArm->SocketOffset = FVector(0.f, 50.f, 50.f);

	m_FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	m_FollowCamera->SetupAttachment(m_CameraArm);
	m_FollowCamera->bUsePawnControlRotation = false;

	static ConstructorHelpers::FClassFinder<AWeapon> DefaultWeapon(TEXT("Blueprint'/Game/Game/Blueprints/Weapon/SubmachineGun/SubmachineGun.SubmachineGun_C'"));
	if (DefaultWeapon.Succeeded())
		m_DefaultWeapon = DefaultWeapon.Class;

	static ConstructorHelpers::FObjectFinder<UAnimMontage> FireAnimMontage(TEXT("AnimMontage'/Game/Game/Blueprints/Player/Animation/FireMontage.FireMontage'"));
	if (FireAnimMontage.Succeeded())
		m_FireAnimMontage = FireAnimMontage.Object;

	static ConstructorHelpers::FObjectFinder<UAnimMontage> ReloadAnimMontage(TEXT("AnimMontage'/Game/Game/Blueprints/Player/Animation/ReloadMontage.ReloadMontage'"));
	if (ReloadAnimMontage.Succeeded())
		m_ReloadAnimMontage = ReloadAnimMontage.Object;

	static ConstructorHelpers::FObjectFinder<UAnimMontage> DamageAnimMontage(TEXT("AnimMontage'/Game/Game/Blueprints/Player/Animation/DamageMontage.DamageMontage'"));
	if (DamageAnimMontage.Succeeded())
		m_DamageAnimMontage = DamageAnimMontage.Object;

	static ConstructorHelpers::FObjectFinder<UAnimMontage> EquipWeaponMontage(TEXT("AnimMontage'/Game/Game/Blueprints/Player/Animation/EquipWeaponMontage.EquipWeaponMontage'"));
	if (EquipWeaponMontage.Succeeded())
		m_EquipWeaponAnimMontage = EquipWeaponMontage.Object;
}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	OnTakeAnyDamage.AddDynamic(this, &APlayerCharacter::ReceiveDamage);
	
	APlayerCameraManager* CameraManager = Cast<APlayerCharacterController>(Controller)->PlayerCameraManager;
	if (CameraManager)
	{
		CameraManager->ViewPitchMax = 20.f;
		CameraManager->ViewPitchMin = -30.f;
	}

	if (m_FollowCamera)
	{
		m_DefaultCameraFOV = m_FollowCamera->FieldOfView;
		m_CurCameraFOV = m_DefaultCameraFOV;
	}

	if (GetCharacterMovement())
		m_DefaultMoveSpeed = GetCharacterMovement()->MaxWalkSpeed;
	
	AWeapon* DefaultWeapon = SpawnDefaultWeapon();
	EquipWeapon(DefaultWeapon);

	m_Controller = m_Controller == nullptr ? Cast<APlayerCharacterController>(Controller) : m_Controller;
	if (m_Controller)
	{
		m_HUD = m_HUD == nullptr ? Cast<AShootingHUD>(m_Controller->GetHUD()) : m_HUD;
		m_Controller->AddWeapon(DefaultWeapon);
		m_Controller->AddWeapon(SpawnDefaultWeapon());
	}

	PlayMontage(m_EquipWeaponAnimMontage, TEXT("Equip"));
}

void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdateCameraFOV(DeltaTime);
	UpdateCrosshairHUD(DeltaTime);
	UpdateAimOffset(DeltaTime);
	UpdateTraceHitResult();
	UpdateCrosshairColor();
	UpdateStamina(DeltaTime);
}

void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &APlayerCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &APlayerCharacter::MoveRight);
	PlayerInputComponent->BindAxis("Turn", this, &APlayerCharacter::Turn);
	PlayerInputComponent->BindAxis("LookUp", this, &APlayerCharacter::LookUp);

	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &APlayerCharacter::FireButtonPressed);
	PlayerInputComponent->BindAction("Fire", IE_Released, this, &APlayerCharacter::FireButtonReleased);

	PlayerInputComponent->BindAction("Aiming", IE_Pressed, this, &APlayerCharacter::AimingButtonPressed);
	PlayerInputComponent->BindAction("Aiming", IE_Released, this, &APlayerCharacter::AimingButtonReleased);

	PlayerInputComponent->BindAction("Reload", IE_Pressed, this, &APlayerCharacter::ReloadButtonPressed);

	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &APlayerCharacter::SprintButtonPressed);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &APlayerCharacter::SprintButtonReleased);

	PlayerInputComponent->BindAction("Key1", IE_Pressed, this, &APlayerCharacter::Key1ButtonPressed);
	PlayerInputComponent->BindAction("Key2", IE_Pressed, this, &APlayerCharacter::Key2ButtonPressed);

}

void APlayerCharacter::MoveForward(float _Value)
{
	if (_Value == 0.f)
	{
		if (m_IsSprint)
			StopSprint();

		return;
	}
	
	m_IsMoveForward = _Value == 1.f ? true : false;

	const FRotator Rotation(Controller->GetControlRotation());
	const FRotator RotationYaw(0.f, Rotation.Yaw, 0.f);

	const FVector Direction(FRotationMatrix{ RotationYaw }.GetUnitAxis(EAxis::X));
	AddMovementInput(Direction, _Value);
}

void APlayerCharacter::MoveRight(float _Value)
{
	if (_Value == 0.f || m_State == EPlayerState::Sprint)
		return;

	const FRotator Rotation(Controller->GetControlRotation());
	const FRotator RotationYaw(0.f, Rotation.Yaw, 0.f);

	const FVector Direction(FRotationMatrix{ RotationYaw }.GetUnitAxis(EAxis::Y));
	AddMovementInput(Direction, _Value);
}

void APlayerCharacter::Turn(float _Value)
{
	float TurnRate = m_IsAiming ? m_AimingTurnRate : m_TurnRate;

	AddControllerYawInput(_Value * TurnRate * GetWorld()->GetDeltaSeconds());
}

void APlayerCharacter::LookUp(float _Value)
{
	float LookUpRate = m_IsAiming ? m_AimingLookUpRate : m_LookUpRate;

	AddControllerPitchInput(_Value * LookUpRate * GetWorld()->GetDeltaSeconds());
}

void APlayerCharacter::FireButtonPressed()
{
	m_FireButtonPress = true;

	Fire();
}

void APlayerCharacter::FireButtonReleased()
{
	m_FireButtonPress = false;
}

void APlayerCharacter::AimingButtonPressed()
{
	m_AimingButton = true;

	Aiming();
}

void APlayerCharacter::AimingButtonReleased()
{
	m_AimingButton = false;

	StopAiming();
}

void APlayerCharacter::ReloadButtonPressed()
{
	Reloading();
}

void APlayerCharacter::SprintButtonPressed()
{
	m_SprintButtonPress = true;

	Sprint();
}

void APlayerCharacter::SprintButtonReleased()
{
	m_SprintButtonPress = false;
}

void APlayerCharacter::Key1ButtonPressed()
{
	ChangeWeapon(m_EquipWeapon, 0);
}

void APlayerCharacter::Key2ButtonPressed()
{
	ChangeWeapon(m_EquipWeapon, 1);
}

AWeapon* APlayerCharacter::SpawnDefaultWeapon()
{
	if (m_DefaultWeapon == nullptr)
		return nullptr;

	return GetWorld()->SpawnActor<AWeapon>(m_DefaultWeapon);
}

void APlayerCharacter::EquipWeapon(AWeapon* _Weapon)
{
	if (_Weapon == nullptr)
		return;

	const USkeletalMeshSocket* HandSocket = GetMesh()->GetSocketByName(FName("RightHandSocket"));
	if (HandSocket)
		HandSocket->AttachActor(_Weapon, GetMesh());

	m_EquipWeapon = _Weapon;
	m_EquipWeapon->SetOwner(this);
	
	m_Controller = m_Controller == nullptr ? Cast<APlayerCharacterController>(Controller) : m_Controller;
	if (m_Controller)
	{
		m_HUD = m_HUD == nullptr ? Cast<AShootingHUD>(m_Controller->GetHUD()) : m_HUD;
		if (m_HUD)
		{
			m_HUD->SetCrosshairType(m_EquipWeapon->GetCrosshairType());
			m_HUD->SetWeaponName(_Weapon->GetWeaponName());
		}
	}
}

void APlayerCharacter::Aiming()
{
	if (m_State == EPlayerState::Reloading)
		return;

	m_IsAiming = true;

	GetCharacterMovement()->MaxWalkSpeed = m_AimingMoveSpeed;
}

void APlayerCharacter::StopAiming()
{
	m_IsAiming = false;

	GetCharacterMovement()->MaxWalkSpeed = m_DefaultMoveSpeed;
}

void APlayerCharacter::Fire()
{
	if (m_EquipWeapon == nullptr)
		return;

	if (m_State != EPlayerState::Idle)
	{
		if (m_State == EPlayerState::Sprint)
			StopSprint();
	}

	m_Controller = m_Controller == nullptr ? Cast<APlayerCharacterController>(Controller) : m_Controller;
	if (m_EquipWeapon->AmmoEmpty())
	{
		if (m_Controller && m_Controller->AmmoMapEmpty(m_EquipWeapon->GetWeaponType()) == false)
			Reloading();

		return;
	}

	m_CrosshairRecoilValue = FMath::Min(m_CrosshairRecoilValue + m_EquipWeapon->GetCrosshairRecoil(), m_CrosshairRecoilMaxValue);
	m_EquipWeapon->Fire(m_CrosshairSpread, m_TraceHitResult);

	if (m_FireAnimMontage)
		PlayMontage(m_FireAnimMontage, TEXT("StartFire"));

	if (m_EquipWeapon->IsAutoFire())
		GetWorldTimerManager().SetTimer(m_AutoFireTimer, this, &APlayerCharacter::FireTimerEnd, m_EquipWeapon->GetAutoFireDelay());

	if (m_Controller)
		m_Controller->SubAmmo();
}

void APlayerCharacter::FireTimerEnd()
{
	if (m_EquipWeapon == nullptr)
		return;

	if (m_FireButtonPress)
		Fire();
}

void APlayerCharacter::Reloading()
{
	if (m_EquipWeapon == nullptr)
		return;

	if (m_State != EPlayerState::Idle)
	{
		if (m_State == EPlayerState::Sprint)
			StopSprint();
		else
			return;
	}

	if (m_Controller == nullptr || m_Controller->AmmoMapEmpty(m_EquipWeapon->GetWeaponType()) || m_EquipWeapon->AmmoFull())
		return;

	m_State = EPlayerState::Reloading;
	m_IsAiming = false;

	if (m_ReloadAnimMontage)
		PlayMontage(m_ReloadAnimMontage, m_EquipWeapon->GetReloadSectionName());
}

void APlayerCharacter::Sprint()
{
	if (m_IsMoveForward == false)
		return;

	if (m_State != EPlayerState::Idle && m_State != EPlayerState::Sprint)
		return;

	if (m_IsSprint)
	{
		StopSprint();
	}
	else
	{
		m_StaminaRestore = false;

		GetCharacterMovement()->MaxWalkSpeed = m_SprintMoveSpeed;
		m_State = EPlayerState::Sprint;
		m_IsSprint = true;
	}
}

void APlayerCharacter::StopSprint()
{
	m_IsSprint = false;
	GetCharacterMovement()->MaxWalkSpeed = m_DefaultMoveSpeed;

	m_State = EPlayerState::Idle;

	GetWorldTimerManager().SetTimer(m_StaminaRetoreTimer, this, &APlayerCharacter::StartStaminaRestore, m_StaminaRestoreStartTime);
}

void APlayerCharacter::DamageTimerEnd()
{
	m_CanDamage = true;
}

void APlayerCharacter::ReloadFinish()
{
	m_State = EPlayerState::Idle;

	if (m_AimingButton)
		m_IsAiming = true;

	if (m_Controller)
		m_Controller->ReloadFinish();

	if (m_FireButtonPress)
		Fire();
}

void APlayerCharacter::EquipFinish()
{
	m_State = EPlayerState::Idle;

	if (m_FireButtonPress)
		Fire();

	if (m_AimingButton)
		Aiming();
}

void APlayerCharacter::ChangeWeapon(AWeapon* _Weapon, int32 _SlotIndex)
{
	if (m_State == EPlayerState::Sprint)
		return;

	if (m_IsAiming)
		StopAiming();

	m_Controller = m_Controller == nullptr ? Cast<APlayerCharacterController>(Controller) : m_Controller;
	if (m_Controller)
		m_Controller->ChangeWeapon(_Weapon, _SlotIndex);
}

void APlayerCharacter::UpdateCameraFOV(float _DeltaTime)
{
	if (m_EquipWeapon == nullptr || m_FollowCamera == nullptr)
		return;

	if (m_IsAiming)
		m_CurCameraFOV = FMath::FInterpTo(m_CurCameraFOV, m_EquipWeapon->GetCameraZoomFOV(), _DeltaTime, m_EquipWeapon->GetCameraZoomSpeed());
	else if (m_IsSprint)
		m_CurCameraFOV = FMath::FInterpTo(m_CurCameraFOV, m_SprintCameraFOV, _DeltaTime, m_SprintCameraZoomSpeed);
	else
		m_CurCameraFOV = FMath::FInterpTo(m_CurCameraFOV, m_DefaultCameraFOV, _DeltaTime, m_ZoomReleaseSpeed);

	m_FollowCamera->FieldOfView = m_CurCameraFOV;
}

void APlayerCharacter::UpdateCrosshairHUD(float _DeltaTime)
{
	m_Controller = m_Controller == nullptr ? Cast<APlayerCharacterController>(Controller) : m_Controller;
	if (m_Controller == nullptr)
		return;

	m_HUD = m_HUD == nullptr ? Cast<AShootingHUD>(m_Controller->GetHUD()) : m_HUD;
	if (m_HUD == nullptr)
		return;

	FVector2D WalkSpeedRange(0.f, GetCharacterMovement()->MaxWalkSpeed);
	FVector2D VelocityMultiplierRange(0.f, 0.3f);
	FVector Velocity = GetVelocity();
	Velocity.Z = 0.f;
	
	float CrosshairMoveValue = FMath::GetMappedRangeValueClamped(WalkSpeedRange, VelocityMultiplierRange, Velocity.Size());
	m_CrosshairAimingValue = m_IsAiming ? FMath::FInterpTo(m_CrosshairAimingValue, 0.5f, _DeltaTime, 30.f) : FMath::FInterpTo(m_CrosshairAimingValue, 0.f, _DeltaTime, 30.f);
	m_CrosshairRecoilValue = FMath::FInterpTo(m_CrosshairRecoilValue, 0.f, _DeltaTime, 10.f);
	
	float CrosshairSpread =	0.5f + CrosshairMoveValue -	m_CrosshairAimingValue + m_CrosshairRecoilValue;
	m_CrosshairSpread = CrosshairSpread * m_CrosshairSpreadMultiple;

	m_HUD->SetCrosshairSpread(m_CrosshairSpread);
}

void APlayerCharacter::UpdateAimOffset(float _DeltaTime)
{
	if (m_EquipWeapon == nullptr)
		return;

	FVector MovementVelocity = GetVelocity();
	MovementVelocity.Z = 0.f;
	float Speed = MovementVelocity.Size();

	if (Speed == 0.f)
	{
		m_UseRotationRootBone = true;
		if (m_TurnState == ETurnState::None)
		{
			FRotator CurAimRotation = FRotator(0.f, GetBaseAimRotation().Yaw, 0.f);
			FRotator DeltaAimRotation = UKismetMathLibrary::NormalizedDeltaRotator(CurAimRotation, m_StartAimRotation);
			m_AOYaw = DeltaAimRotation.Yaw;
		}

		TurnInPlace(_DeltaTime);
	}
	else if (0.f < Speed)
	{
		m_UseRotationRootBone = false;
		m_StartAimRotation = FRotator(0.f, GetBaseAimRotation().Yaw, 0.f);
		m_AOYaw = 0.f;

		m_TurnState = ETurnState::None;
	}

	m_AOPitch = GetBaseAimRotation().Pitch;
	if (90.f < m_AOPitch)
	{
		FVector2D InRange(270.f, 360.f);
		FVector2D OutRange(-90.f, 0.f);
		m_AOPitch = FMath::GetMappedRangeValueClamped(InRange, OutRange, m_AOPitch);
	}
}

void APlayerCharacter::UpdateTraceHitResult()
{
	m_Controller = m_Controller == nullptr ? Cast<APlayerCharacterController>(Controller) : m_Controller;
	if (m_Controller == nullptr || m_EquipWeapon == nullptr)
		return;

	FVector2D ViewportSize;
	if (GEngine && GEngine->GameViewport)
		GEngine->GameViewport->GetViewportSize(ViewportSize);

	FVector2D CrosshairLocation(ViewportSize.X * 0.5f, ViewportSize.Y * 0.5f);
	FVector CrosshairWorldPosition;
	FVector CrosshairWorldDirection;

	bool ScreenToWorld = UGameplayStatics::DeprojectScreenToWorld(
		m_Controller,
		CrosshairLocation,
		CrosshairWorldPosition,
		CrosshairWorldDirection
	);

	if (ScreenToWorld)
	{
		float DistanceToCharacter = (GetActorLocation() - CrosshairWorldPosition).Size();
		FVector Start = CrosshairWorldPosition + CrosshairWorldDirection * DistanceToCharacter;

		FVector End = Start + CrosshairWorldDirection * m_EquipWeapon->GetRange();

		GetWorld()->LineTraceSingleByChannel(
			m_TraceHitResult,
			Start,
			End,
			COLLISION_PLAYERBULLET
		);
	}
}

void APlayerCharacter::UpdateCrosshairColor()
{
	m_HUD = m_HUD == nullptr ? Cast<AShootingHUD>(m_Controller->GetHUD()) : m_HUD;
	if (m_HUD == nullptr)
		return;

	FLinearColor CrosshairColor = FLinearColor::White;
	if (m_TraceHitResult.bBlockingHit)
	{
		AMonster* Monster = Cast<AMonster>(m_TraceHitResult.Actor);
		if (Monster)
			CrosshairColor = FLinearColor::Red;
	}

	if (m_HUD)
		m_HUD->SetCrosshairColor(CrosshairColor);
}

void APlayerCharacter::UpdateStamina(float _DeltaTime)
{
	m_Controller = m_Controller == nullptr ? Cast<APlayerCharacterController>(Controller) : m_Controller;
	if (m_Controller == nullptr)
		return;

	if (m_IsSprint)
		m_Controller->AddStamina(-_DeltaTime * m_StaminaRestoreSpeed);
	else if (m_StaminaRestore)
		m_Controller->AddStamina(_DeltaTime * m_StaminaRestoreSpeed);
}

void APlayerCharacter::StartStaminaRestore()
{
	m_StaminaRestore = true;
}

void APlayerCharacter::TurnInPlace(float _DeltaTime)
{
	if (90.f < m_AOYaw)
		m_TurnState = ETurnState::Right;
	else if (m_AOYaw < -90.f)
		m_TurnState = ETurnState::Left;

	if (m_TurnState != ETurnState::None)
	{
		m_AOYaw = FMath::FInterpTo(m_AOYaw, 0.f, _DeltaTime, 6.f);
		if (FMath::Abs(m_AOYaw) < 1.f)
		{
			m_TurnState = ETurnState::None;
			m_StartAimRotation = FRotator(0.f, GetBaseAimRotation().Yaw, 0.f);
		}
	}
}

void APlayerCharacter::PlayMontage(UAnimMontage* _AnimMontage, FName _SectionName)
{
	if (_AnimMontage == nullptr)
		return;

	UAnimInstance* AnimInst = GetMesh()->GetAnimInstance();
	if (AnimInst)
	{
		AnimInst->Montage_Play(_AnimMontage);
		AnimInst->Montage_JumpToSection(_SectionName);
	}
}

void APlayerCharacter::PlayHitParticle(AActor* _Actor)
{
	AMonster* Monster = Cast<AMonster>(_Actor);
	if (Monster == nullptr)
		return;

	UParticleSystem* HitParticle = Monster->GetHitParticle();
	if (HitParticle == nullptr)
		return;

	const USkeletalMeshSocket* ImpactSocket = GetMesh()->GetSocketByName(TEXT("Impact"));
	if (ImpactSocket)
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitParticle, ImpactSocket->GetSocketTransform(GetMesh()));
}

void APlayerCharacter::ReceiveDamage(AActor* _DamagedActor, float _Damage, const UDamageType* _DamageType, class AController* _InstigatorController, AActor* _DamageCauser)
{
	if (m_CanDamage == false)
		return;

	m_Controller = m_Controller == nullptr ? Cast<APlayerCharacterController>(Controller) : m_Controller;
	if (m_Controller == nullptr)
		return;

	m_Controller->AddHP(-_Damage);

	m_CanDamage = false;
	GetWorldTimerManager().SetTimer(m_DamageTimer, this, &APlayerCharacter::DamageTimerEnd, m_CanDamageTimeRate);
	PlayHitParticle(_DamageCauser);

	if (m_DamageAnimMontage && m_State != EPlayerState::Reloading)
		PlayMontage(m_DamageAnimMontage, TEXT("Damage"));
}