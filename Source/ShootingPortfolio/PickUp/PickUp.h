#pragma once

#include "ShootingPortfolio/global.h"

#include "NiagaraFunctionLibrary.h"
#include "NiagaraSystem.h"
#include "NiagaraComponent.h"

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PickUp.generated.h"

class UNiagaraComponent;
class URotatingMovementComponent;

UCLASS()
class SHOOTINGPORTFOLIO_API APickUp : public AActor
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, Category = Info)
	UStaticMeshComponent* m_GroundMesh;
	
	UPROPERTY(EditAnywhere, Category = Info)
	UNiagaraComponent* m_NiagaraComponent;

	UPROPERTY(EditAnywhere, Category = Info)
	UStaticMeshComponent* m_StaticMesh;

	UPROPERTY(EditAnywhere, Category = Info)
	USphereComponent* m_CollisionSphere;

	UPROPERTY(EditAnywhere, Category = Info)
	UNiagaraSystem* m_PickUpNiagaraSystem;

	UPROPERTY(EditAnywhere, Category = Info)
	USoundCue* m_PickUpSound;

	UPROPERTY(EditAnywhere, Category = Info)
	float m_SpawnCooltime;

	FTimerHandle m_SpawnTimer;
	
public:	
	APickUp();

protected:
	void SpawnTimerEnd();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

public:
	UFUNCTION()
	virtual void OnBeginOverlap(UPrimitiveComponent* _OverlappedComponent, AActor* _OtherActor, UPrimitiveComponent* _OtherComp, int32 _OtherBodyIndex, bool _bFromSweep, const FHitResult& _SweepResult);
};
