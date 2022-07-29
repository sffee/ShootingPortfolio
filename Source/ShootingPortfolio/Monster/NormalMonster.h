#pragma once

#include "CoreMinimal.h"
#include "Monster.h"
#include "NormalMonster.generated.h"

class UMonsterHPBarWidget;

UCLASS()
class SHOOTINGPORTFOLIO_API ANormalMonster : public AMonster
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(VisibleAnywhere)
	UWidgetComponent* m_HPBarWidgetComponent;

	UPROPERTY()
	UMonsterHPBarWidget* m_HPBarWidget;

protected:
	UPROPERTY(EditAnywhere, Category = Particle)
	UParticleSystem* m_DeathParticle;

public:
	ANormalMonster();

public:
	virtual void BeginPlay() override;

protected:
	void UpdateHPBarWidget();

protected:
	virtual void ReceiveDamage(AActor* _DamagedActor, float _Damage, const UDamageType* _DamageType, class AController* _InstigatorController, AActor* _DamageCauser) override;
};