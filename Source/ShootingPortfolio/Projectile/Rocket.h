#pragma once

#include "CoreMinimal.h"
#include "Projectile.h"
#include "Rocket.generated.h"

UCLASS()
class SHOOTINGPORTFOLIO_API ARocket : public AProjectile
{
	GENERATED_BODY()
	
public:
	ARocket();

protected:
	virtual void BeginPlay() override;
	
private:
	void Explosion();

public:
	virtual void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
};
