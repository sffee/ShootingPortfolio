#pragma once

#include "CoreMinimal.h"
#include "Weapon.h"
#include "HitScanWeapon.generated.h"

UCLASS()
class SHOOTINGPORTFOLIO_API AHitScanWeapon : public AWeapon
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditAnywhere, Category = Effect)
	UParticleSystem* m_SmokeBeamParticle;
	
public:
	AHitScanWeapon();

public:
	virtual void Fire(const FHitResult& _HitResult) override;

private:
	void PlaySmokeBeamParticle(const FHitResult& _HitResult);
};