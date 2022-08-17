#include "Weapon.h"

#include "ShootingPortfolio/Player/PlayerCharacter.h"
#include "ShootingPortfolio/UI/DamageText/DamageTextActor.h"

AWeapon::AWeapon()
	: m_Ammo(0)
	, m_Magazine(0)
	, m_InfinityMagazine(false)
	, m_CrosshairRecoil(0.f)
	, m_CameraZoomFOV(0.f)
	, m_CameraZoomSpeed(20.f)
	, m_IsAutoFire(true)
	, m_FireDelay(0.1f)
	, m_Type(EWeaponType::MAX)
	, m_IdleFireBlendWeight(0.75f)
	, m_AimingFireBlendWeight(0.75f)
	, m_InventorySlotIndex(0)
	, m_Damage(0)
	, m_HeadDamageRate(1.5f)
	, m_Range(1000.f)
{
	PrimaryActorTick.bCanEverTick = true;

	m_Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
	SetRootComponent(m_Mesh);
	m_Mesh->SetEnableGravity(false);

	m_Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	m_Mesh->bReceivesDecals = false;
}

void AWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}

void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AWeapon::Fire(const FHitResult& _TraceHitResult)
{
	PlaySound(m_FireSound);
	PlayMuzzleFlashParticle();
	PlayCameraShake();
	PlayFireAnimation();
}

void AWeapon::SetAmmo(int32 _Ammo)
{
	m_Ammo = _Ammo;
}

void AWeapon::AddAmmo(int32 _Ammo)
{
	m_Ammo += _Ammo;
}

void AWeapon::PlaySound(USoundCue* _Sound)
{
	if (_Sound == nullptr)
		return;

	UGameplayStatics::PlaySound2D(this, _Sound);
}

void AWeapon::PlayMuzzleFlashParticle()
{
	if (m_MuzzleFlashParticle == nullptr)
		return;

	const USkeletalMeshSocket* BarrelSocket = m_Mesh->GetSocketByName(FName("BarrelSocket"));
	if (BarrelSocket)
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), m_MuzzleFlashParticle, BarrelSocket->GetSocketTransform(m_Mesh));
}

void AWeapon::PlayCameraShake()
{
	if (m_FireCameraShake == nullptr)
		return;

	APlayerCharacter* Player = Cast<APlayerCharacter>(GetOwner());
	if (Player == nullptr)
		return;
	
	APlayerController* Controller = Cast<APlayerController>(Player->GetController());
	if (Controller == nullptr)
		return;

	Controller->ClientStartCameraShake(m_FireCameraShake);
}

void AWeapon::PlayFireAnimation()
{
	if (m_FireAnimation == nullptr)
		return;

	m_Mesh->PlayAnimation(m_FireAnimation, false);
}

void AWeapon::PlayHitParticle(const FVector& _Location)
{
	if (m_HitParticle == nullptr)
		return;

	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), m_HitParticle, _Location);
}

void AWeapon::CanFireTimerEnd()
{
	m_CanFire = true;
}