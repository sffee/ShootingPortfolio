#pragma once

#include "CoreMinimal.h"
#include "PickUp.h"
#include "PickUp_Ammo.generated.h"

UCLASS()
class SHOOTINGPORTFOLIO_API APickUp_Ammo : public APickUp
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditAnywhere)
	float m_AmmoAmountRate;

	UPROPERTY(EditAnywhere)
	float m_RotationSpeed;

public:
	APickUp_Ammo();

private:
	void RotationStaticMesh(float _DeltaTime);

public:
	virtual void Tick(float DeltaTime) override;

public:
	virtual void OnBeginOverlap(UPrimitiveComponent* _OverlappedComponent, AActor* _OtherActor, UPrimitiveComponent* _OtherComp, int32 _OtherBodyIndex, bool _bFromSweep, const FHitResult& _SweepResult);
};
