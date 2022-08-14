#pragma once

#include "CoreMinimal.h"
#include "Weapon.h"
#include "HitScanWeapon.generated.h"

UCLASS()
class SHOOTINGPORTFOLIO_API AHitScanWeapon : public AWeapon
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditAnywhere, Category = Particle)
	UParticleSystem* m_SmokeBeamParticle;

	FHitResult m_HitResult;
	
public:
	AHitScanWeapon();

public:
	virtual void Fire(const FHitResult& _TraceHitResult) override;

private:
	void CalcHitResult();

private:
	void PlaySmokeBeamParticle();
	void ApplyDamage();
};