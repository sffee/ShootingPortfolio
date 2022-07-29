#pragma once

#include "CoreMinimal.h"
#include "../NormalMonster.h"
#include "Minions.generated.h"

UCLASS()
class SHOOTINGPORTFOLIO_API AMinions : public ANormalMonster
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditAnywhere, Category = Attack)
	float m_AttackDamage;

public:
	AMinions();
	
public:
	virtual void BeginPlay() override;
	virtual void Destroyed() override;

public:
	virtual void OnBeginOverlap(UPrimitiveComponent* _PrimitiveComponent, AActor* _OtherActor, UPrimitiveComponent* _OtherComp, int32 _OtherBodyIndex, bool _bFromSweep, const FHitResult& _SweepResult) override;;
};
