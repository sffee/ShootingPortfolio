#pragma once

#include "CoreMinimal.h"
#include "../NormalMonster.h"
#include "Steel.generated.h"

UENUM(BlueprintType)
enum class ESteelState : uint8
{
	Shield,
	ShieldBreak,
	Idle,

	MAX
};

class ASteelShield;

UCLASS()
class SHOOTINGPORTFOLIO_API ASteel : public ANormalMonster
{
	GENERATED_BODY()
	
private:
	TSubclassOf<AActor> m_ShieldClass;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess))
	ASteelShield* m_Shield;

	UPROPERTY(EditAnywhere)
	UAnimMontage* m_ShieldBreakMontage;

private:
	UPROPERTY(VisibleAnywhere, Category = State)
	ESteelState m_SteelState;
	
public:
	ASteel();

public:
	virtual void BeginPlay() override;
	virtual void Destroyed() override;
	virtual void RightWeaponCollisionEnable() override;
	virtual void RightWeaponCollisionDisable() override;

private:
	void SpawnAndEquipShield();

public:
	void SetSteelState(ESteelState _State);

public:
	FORCEINLINE UAnimMontage* GetShieldBreakMontage() const { return m_ShieldBreakMontage; }
	FORCEINLINE ESteelState GetSteelState() const { return m_SteelState; }
};