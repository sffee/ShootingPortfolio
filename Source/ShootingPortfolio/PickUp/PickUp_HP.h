#pragma once

#include "CoreMinimal.h"
#include "PickUp.h"
#include "PickUp_HP.generated.h"

UCLASS()
class SHOOTINGPORTFOLIO_API APickUp_HP : public APickUp
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditAnywhere)
	float m_HealAmount;
	
public:
	APickUp_HP();

public:
	virtual void OnBeginOverlap(UPrimitiveComponent* _OverlappedComponent, AActor* _OtherActor, UPrimitiveComponent* _OtherComp, int32 _OtherBodyIndex, bool _bFromSweep, const FHitResult& _SweepResult);
};
