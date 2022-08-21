#pragma once

#include "CoreMinimal.h"
#include "../Monster.h"
#include "Khaimera.generated.h"

class AWarningMark;
class ASpawnMonsterCircle;
class UBossMonsterHPBarWidget;

UCLASS()
class SHOOTINGPORTFOLIO_API AKhaimera : public AMonster
{
	GENERATED_BODY()
	
private:
	TSubclassOf<AActor> m_WarningMarkClass;
	TSubclassOf<AActor> m_SpawnMonsterCircleClass;

	UPROPERTY()
	AWarningMark* m_WarningMark;

	UPROPERTY(EditAnywhere)
	UAnimMontage* m_DeathMontage;

	UPROPERTY(EditAnywhere)
	float m_JumpAttackRange;

	UPROPERTY(EditAnywhere)
	float m_JumpAttackOffset;

private:
	TSubclassOf<UUserWidget> m_BossMonsterHPBarWidgetClass;

	UPROPERTY()
	UBossMonsterHPBarWidget* m_BossMonsterHPBarWidget;

private:
	bool m_SpawnMonsterPattern;

public:
	AKhaimera();

public:
	virtual void BeginPlay() override;

public:
	void CreateWarningMark(const FVector& _Location);
	void DestroyWarningMark();
	void JumpAttack();

private:
	void UpdateHPBarWidget();
	void UpdateShieldBarWidget();

public:
	virtual float TakeDamage(float _DamageAmount, FDamageEvent const& _DamageEvent, AController* _EventInstigator, AActor* _DamageCauser) override;

public:
	FORCEINLINE TSubclassOf<AActor> GetSpawnMonsterCircleClass() const { return m_SpawnMonsterCircleClass; }
};