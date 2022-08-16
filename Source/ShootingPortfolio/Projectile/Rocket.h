#pragma once

#include "CoreMinimal.h"
#include "Projectile.h"
#include "Rocket.generated.h"

UCLASS()
class SHOOTINGPORTFOLIO_API ARocket : public AProjectile
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, Category = Hit)
	float m_DamageRadius;
	
public:
	ARocket();

protected:
	virtual void BeginPlay() override;
	
private:
	void Explosion();

public:
	virtual void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	virtual void Destroyed() override;
};
