#include "PlayerCharacter.h"

#include "PlayerCharacterController.h"
#include "ShootingPortfolio/Weapon/Weapon.h"
#include "ShootingPortfolio/UI/ShootingHUD.h"

APlayerCharacter::APlayerCharacter()
	: m_TurnRate(45.f)
	, m_AimingTurnRate(20.f)
	, m_LookUpRate(45.f)
	, m_AimingLookUpRate(20.f)
	, m_DefaultMoveSpeed(600.f)
	, m_AimingMoveSpeed(300.f)
	, m_State(EPlayerState::Idle)
	, m_DefaultCameraFOV(0.f)
	, m_CurCameraFOV(0.f)
	, m_ZoomReleaseSpeed(20.f)
	, m_AimingButton(false)
	, m_IsAiming(false)
	, m_FireButtonPress(false)
	, m_CrosshairAimingValue(0.f)
	, m_CrosshairRecoilValue(0.f)
	, m_CrosshairSpreadMultiple(16.f)
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

	m_CameraArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	m_CameraArm->SetupAttachment(RootComponent);
	m_CameraArm->TargetArmLength = 300.f;
	m_CameraArm->bUsePawnControlRotation = true;
	m_CameraArm->SocketOffset = FVector(0.f, 50.f, 50.f);

	m_FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	m_FollowCamera->SetupAttachment(m_CameraArm);
	m_FollowCamera->bUsePawnControlRotation = false;

	static ConstructorHelpers::FClassFinder<AWeapon> DefaultWeapon(TEXT("Blueprint'/Game/Game/Blueprints/Weapon/SubmachineGun.SubmachineGun_C'"));
	if (DefaultWeapon.Succeeded())
		m_DefaultWeapon = DefaultWeapon.Class;

	static ConstructorHelpers::FObjectFinder<UAnimMontage> FireAnimMontage(TEXT("AnimMontage'/Game/Game/Asset/Animation/Player/FireMontage.FireMontage'"));
	if (FireAnimMontage.Succeeded())
		m_FireAnimMontage = FireAnimMontage.Object;

	static ConstructorHelpers::FObjectFinder<UAnimMontage> ReloadAnimMontage(TEXT("AnimMontage'/Game/Game/Asset/Animation/Player/ReloadMontage.ReloadMontage'"));
	if (ReloadAnimMontage.Succeeded())
		m_ReloadAnimMontage = ReloadAnimMontage.Object;
}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
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

	m_Controller = m_Controller == nullptr ? Cast<APlayerCharacterController>(Controller) : m_Controller;
	if (m_Controller)
		m_HUD = m_HUD == nullptr ? Cast<AShootingHUD>(m_Controller->GetHUD()) : m_HUD;

	EquipWeapon(SpawnDefaultWeapon());
}

void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdateCameraFOV(DeltaTime);
	UpdateTraceHitResult();
	UpdateCrosshairHUD(DeltaTime);
	UpdateAimOffset(DeltaTime);
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

	PlayerInputComponent->BindAction("Reload", IE_Pressed, this, &APlayerCharacter::ReloadPressed);

}

void APlayerCharacter::MoveForward(float _Value)
{
	if (_Value == 0.f)
		return;

	const FRotator Rotation(Controller->GetControlRotation());
	const FRotator RotationYaw(0.f, Rotation.Yaw, 0.f);

	const FVector Direction(FRotationMatrix{ RotationYaw }.GetUnitAxis(EAxis::X));
	AddMovementInput(Direction, _Value);
}

void APlayerCharacter::MoveRight(float _Value)
{
	if (_Value == 0.f)
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

void APlayerCharacter::ReloadPressed()
{
	Reloading();
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
	if (m_EquipWeapon == nullptr || m_State != EPlayerState::Idle)
		return;

	m_Controller = m_Controller == nullptr ? Cast<APlayerCharacterController>(Controller) : m_Controller;
	if (m_EquipWeapon->AmmoEmpty())
	{
		if (m_Controller && m_Controller->AmmoMapEmpty(m_EquipWeapon->GetWeaponType()) == false)
			Reloading();

		return;
	}

	m_CrosshairRecoilValue = m_EquipWeapon->GetCrosshairRecoil();
	m_EquipWeapon->Fire(m_TraceHitResult);

	if (m_FireAnimMontage)
		PlayMontage(m_FireAnimMontage, TEXT("StartFire"));

	m_State = EPlayerState::Fire;

	if (m_EquipWeapon->IsAutoFire())
		GetWorldTimerManager().SetTimer(m_AutoFireTimer, this, &APlayerCharacter::FireTimerEnd, m_EquipWeapon->GetAutoFireDelay());

	if (m_Controller)
		m_Controller->SubAmmo();
}

void APlayerCharacter::FireTimerEnd()
{
	m_State = EPlayerState::Idle;

	if (m_EquipWeapon == nullptr)
		return;

	if (m_FireButtonPress)
		Fire();
}

void APlayerCharacter::Reloading()
{
	if (m_EquipWeapon == nullptr || (m_State != EPlayerState::Idle && m_State != EPlayerState::Fire))
		return;

	if (m_Controller == nullptr || m_Controller->AmmoMapEmpty(m_EquipWeapon->GetWeaponType()) || m_EquipWeapon->AmmoFull())
		return;

	m_State = EPlayerState::Reloading;
	m_IsAiming = false;

	if (m_ReloadAnimMontage)
		PlayMontage(m_ReloadAnimMontage, m_EquipWeapon->GetReloadSectionName());
}

void APlayerCharacter::ReloadFinish()
{
	m_State = EPlayerState::Idle;

	if (m_AimingButton)
		m_IsAiming = true;

	if (m_Controller)
		m_Controller->ReloadFinish();
}

void APlayerCharacter::UpdateCameraFOV(float _DeltaTime)
{
	if (m_EquipWeapon == nullptr || m_FollowCamera == nullptr)
		return;

	m_CurCameraFOV = m_IsAiming ?
		FMath::FInterpTo(m_CurCameraFOV, m_EquipWeapon->GetCameraZoomFOV(), _DeltaTime, m_EquipWeapon->GetCameraZoomSpeed()) :
		FMath::FInterpTo(m_CurCameraFOV, m_DefaultCameraFOV, _DeltaTime, m_ZoomReleaseSpeed);

	m_FollowCamera->FieldOfView = m_CurCameraFOV;
}

void APlayerCharacter::UpdateTraceHitResult()
{
	FVector2D ViewportSize;
	if (GEngine && GEngine->GameViewport)
		GEngine->GameViewport->GetViewportSize(ViewportSize);

	FVector2D CrosshairLocation(ViewportSize.X * 0.5f, ViewportSize.Y * 0.5f);
	FVector CrosshairWorldPosition;
	FVector CrosshairWorldDirection;
	
	bool ScreenToWorld = UGameplayStatics::DeprojectScreenToWorld(
		UGameplayStatics::GetPlayerController(this, 0),
		CrosshairLocation,
		CrosshairWorldPosition,
		CrosshairWorldDirection
	);

	if (ScreenToWorld)
	{
		FVector Start = CrosshairWorldPosition;
		float DistanceToCharacter = (GetActorLocation() - Start).Size();
		Start += CrosshairWorldDirection * (DistanceToCharacter + 100.f);

		FVector End = Start + CrosshairWorldDirection * 80000.f;

		GetWorld()->LineTraceSingleByChannel(
			m_TraceHitResult,
			Start,
			End,
			COLLISION_PLAYERBULLET
		);
	}
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
	FVector2D VelocityMultiplierRange(0.f, 1.f);
	FVector Velocity = GetVelocity();
	Velocity.Z = 0.f;
	
	float CrosshairMoveValue = FMath::GetMappedRangeValueClamped(WalkSpeedRange, VelocityMultiplierRange, Velocity.Size());
	m_CrosshairAimingValue = m_AimingButton ? FMath::FInterpTo(m_CrosshairAimingValue, 0.5f, _DeltaTime, 30.f) : FMath::FInterpTo(m_CrosshairAimingValue, 0.f, _DeltaTime, 30.f);
	m_CrosshairRecoilValue = FMath::FInterpTo(m_CrosshairRecoilValue, 0.f, _DeltaTime, 40.f);
	
	float CrosshairSpread =	0.5f + CrosshairMoveValue -	m_CrosshairAimingValue + m_CrosshairRecoilValue;
	CrosshairSpread *= m_CrosshairSpreadMultiple;

	m_HUD->SetCrosshairSpread(CrosshairSpread);
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