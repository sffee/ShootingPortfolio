#pragma once

#include "CoreMinimal.h"
#include "Weapon.h"
#include "ProjectileWeapon.generated.h"

class AProjectile;

UCLASS()
class SHOOTINGPORTFOLIO_API AProjectileWeapon : public AWeapon
{
	GENERATED_BODY()
	
public:
	virtual void Fire(const FHitResult& _TraceHitResult) override;

protected:
	UPROPERTY(EditAnywhere, Category = Projectile)
	TSubclassOf<AProjectile> m_Projectile;
};