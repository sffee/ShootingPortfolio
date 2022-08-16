#pragma once

#include "ShootingPortfolio/global.h"
#include "GameFramework/ProjectileMovementComponent.h"

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Projectile.generated.h"

class UProjectileMovementComponent;
class UNiagaraComponent;
class UNiagaraSystem;

UCLASS()
class SHOOTINGPORTFOLIO_API AProjectile : public AActor
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditAnywhere, Category = Info)
	UBoxComponent* m_CollisionBox;

	UPROPERTY(EditAnywhere, Category = Info)
	UStaticMeshComponent* m_Mesh;

	UPROPERTY(EditAnywhere, Category = Info)
	UProjectileMovementComponent* m_ProjectileMovementComponent;

	UPROPERTY(EditAnywhere, Category = Info)
	UParticleSystem* m_ImpactParticle;

	UPROPERTY(EditAnywhere, Category = Info)
	USoundCue* m_ImpactSound;

	UPROPERTY(EditAnywhere, Category = Info)
	UNiagaraComponent* m_TrailSystemComponent;

	UPROPERTY(EditAnywhere, Category = Info)
	UNiagaraSystem* m_TrailSystem;

	UPROPERTY(EditAnywhere, Category = Info)
	float m_LifeSpan;
	
protected:
	UPROPERTY(EditAnywhere, Category = Hit)
	float m_Damage;
	
public:	
	AProjectile();

protected:
	virtual void BeginPlay() override;

protected:
	void SpawnTrailSystem();

public:
	UFUNCTION()
	virtual void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	virtual void Destroyed() override;

public:
	FORCEINLINE void SetDamage(float _Damage) { m_Damage = _Damage; }
};
