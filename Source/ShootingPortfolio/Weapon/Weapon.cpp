#include "Weapon.h"

AWeapon::AWeapon()
	: m_Ammo(0)
	, m_Magazine(0)
	, m_CrosshairRecoil(0.f)
	, m_CameraZoomFOV(0.f)
	, m_CameraZoomSpeed(20.f)
	, m_IsAutoFire(true)
	, m_AutoFireDelay(0.1f)
	, m_Type(EWeaponType::MAX)
{
	PrimaryActorTick.bCanEverTick = true;

	m_Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
	SetRootComponent(m_Mesh);
	m_Mesh->SetEnableGravity(false);
}

void AWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}

void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWeapon::Fire(const FHitResult& _HitResult)
{
	PlaySound(m_FireSound);
	PlayMuzzleFlashParticle();
	PlayHitParticle(_HitResult);
}

void AWeapon::SubAmmo()
{
	m_Ammo--;
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

void AWeapon::PlayHitParticle(const FHitResult& _HitResult)
{
	if (m_HitParticle == nullptr || !_HitResult.bBlockingHit)
		return;

	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), m_HitParticle, _HitResult.ImpactPoint);
}