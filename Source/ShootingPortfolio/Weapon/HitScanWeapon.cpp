#include "HitScanWeapon.h"

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