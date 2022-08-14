#include "ProjectileWeapon.h"

#include "ShootingPortfolio/Projectile/Projectile.h"

void AProjectileWeapon::Fire(const FHitResult& _TraceHitResult)
{
	if (m_Projectile == nullptr)
		return;

	const USkeletalMeshSocket* BarrelSocket = GetMesh()->GetSocketByName(TEXT("BarrelSocket"));
	if (BarrelSocket == nullptr)
		return;

	FVector SocketLocation = BarrelSocket->GetSocketLocation(GetMesh());
	FVector EndLocation = _TraceHitResult.bBlockingHit ? _TraceHitResult.ImpactPoint : _TraceHitResult.TraceEnd;

	FVector ToTarget = EndLocation - SocketLocation;
	FRotator ProjectileRot = ToTarget.Rotation();

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = GetOwner();
	SpawnParams.Instigator = Cast<APawn>(GetOwner());

	AProjectile* Projectile = GetWorld()->SpawnActor<AProjectile>(m_Projectile, SocketLocation, ProjectileRot, SpawnParams);
	Projectile->SetDamage(m_Damage);

	Super::Fire(_TraceHitResult);
}