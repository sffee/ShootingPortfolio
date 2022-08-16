#pragma once

#include "ShootingPortfolio/global.h"

#include "DestructibleComponent.h"
#include "DestructibleMesh.h"
#include "Components/SphereComponent.h"

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BossDoor.generated.h"

UCLASS()
class SHOOTINGPORTFOLIO_API ABossDoor : public AActor
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditAnywhere)
	UDestructibleComponent* m_DestructibleComponent;

	UPROPERTY(EditAnywhere)
	USphereComponent* m_ImpactSphere;

	UPROPERTY(EditAnywhere)
	float m_Damage;

	UPROPERTY(EditAnywhere)
	float m_Time;
	
public:	
	ABossDoor();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

private:
	UFUNCTION(BlueprintCallable, meta = (AllowPrivateAccess = "true"))
	void DoorDestruction();
};
