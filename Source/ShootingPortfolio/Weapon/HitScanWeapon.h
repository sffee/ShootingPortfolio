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
	virtual void Fire(float _Spread, const FHitResult& _TargetHitResult) override;

private:
	void CalcHitResult(float _Spread, const FHitResult& _TargetHitResult);

private:
	void PlaySmokeBeamParticle();
	void ApplyDamage();
};