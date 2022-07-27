#include "HitScanWeapon.h"

#include "ShootingPortfolio/Monster/Monster.h"

AHitScanWeapon::AHitScanWeapon()
{
	const ConstructorHelpers::FObjectFinder<UParticleSystem> SmokeBeam(TEXT("ParticleSystem'/Game/Game/Asset/FX/SmokeBeam/P_SmokeTrail.P_SmokeTrail'"));
	if (SmokeBeam.Succeeded())
		m_SmokeBeamParticle = SmokeBeam.Object;
}

void AHitScanWeapon::Fire(const FHitResult& _HitResult)
{
	Super::Fire(_HitResult);

	PlaySmokeBeamParticle(_HitResult);
	ApplyDamage(_HitResult);
}

void AHitScanWeapon::PlaySmokeBeamParticle(const FHitResult& _HitResult)
{
	if (m_SmokeBeamParticle == nullptr || !_HitResult.bBlockingHit)
		return;

	const USkeletalMeshSocket* BarrelSocket = GetMesh()->GetSocketByName(FName("BarrelSocket"));
	if (BarrelSocket)
	{
		UParticleSystemComponent* SmokeBeam = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), m_SmokeBeamParticle, BarrelSocket->GetSocketTransform(GetMesh()));
		if (SmokeBeam)
			SmokeBeam->SetVectorParameter(FName("Target"), _HitResult.ImpactPoint);
	}
}

void AHitScanWeapon::ApplyDamage(const FHitResult& _HitResult)
{
	if (_HitResult.bBlockingHit)
	{
		AMonster* Monster = Cast<AMonster>(_HitResult.Actor);
		APawn* PlayerPawn = Cast<APawn>(GetOwner());
		if (Monster == nullptr || PlayerPawn == nullptr)
			return;

		AController* PlayerController = Cast<APlayerController>(PlayerPawn->GetController());
		if (PlayerController == nullptr)
			return;

		UGameplayStatics::ApplyDamage(Monster, m_Damage, PlayerController, this, UDamageType::StaticClass());
	}
}