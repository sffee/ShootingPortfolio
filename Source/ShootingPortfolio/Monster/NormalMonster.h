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
	virtual float TakeDamage(float _DamageAmount, FDamageEvent const& _DamageEvent, AController* _EventInstigator, AActor* _DamageCauser) override;
	virtual void Destroyed() override;
};