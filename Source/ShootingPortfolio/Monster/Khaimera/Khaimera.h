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

private:
	void UpdateHPBarWidget();
	void UpdateShieldBarWidget();

public:
	virtual void ReceiveDamage(AActor* _DamagedActor, float _Damage, const UDamageType* _DamageType, class AController* _InstigatorController, AActor* _DamageCauser) override;

public:
	FORCEINLINE TSubclassOf<AActor> GetSpawnMonsterCircleClass() const { return m_SpawnMonsterCircleClass; }
};