#pragma once

#include "ShootingPortfolio/global.h"
#include "ShootingPortfolio/StructData.h"

#include <BehaviorTree/BehaviorTree.h>
#include <BehaviorTree/BlackboardComponent.h>

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Monster.generated.h"

DECLARE_DELEGATE_OneParam(FSpawnMonsterDieDelegate, UObject*)

UCLASS()
class SHOOTINGPORTFOLIO_API AMonster : public ACharacter
{
	GENERATED_BODY()
	
public:
	FSpawnMonsterDieDelegate m_MonsterDieDelegate;

protected:
	UPROPERTY(EditAnywhere)
	UBoxComponent* m_RightWeaponCollision;
	
protected:
	UPROPERTY(EditAnywhere, Category = Attack, meta = (DisplayName = "AttackSectionName"))
	TArray<FName> m_AttackSectionNameList;

	UPROPERTY(EditAnywhere, Category = Attack)
	UAnimMontage* m_AttackMontage;

protected:
	UPROPERTY(EditAnywhere, Category = Status)
	FMonsterStatus m_Status;

protected:
	UPROPERTY(EditAnywhere, Category = AI)
	UBehaviorTree* m_BehaviorTree;

	UPROPERTY(EditAnywhere, Category = AI)
	UBlackboardData* m_Blackboard;

protected:
	UPROPERTY(EditAnywhere, Category = Particle)
	UParticleSystem* m_HitParticle;
	
public:
	AMonster();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

public:
	bool PlayAttackIndex(int32 _Index);

protected:
	void PlayMontage(UAnimMontage* _AnimMontage, FName _SectionName);

protected:
	UFUNCTION()
	virtual void ReceiveDamage(AActor* _DamagedActor, float _Damage, const UDamageType* _DamageType, class AController* _InstigatorController, AActor* _DamageCauser);

	UFUNCTION()
	virtual void OnBeginOverlap(UPrimitiveComponent* _PrimitiveComponent, AActor* _OtherActor, UPrimitiveComponent* _OtherComp, int32 _OtherBodyIndex, bool _bFromSweep, const FHitResult& _SweepResult);

public:
	FORCEINLINE UBehaviorTree* GetBehaviorTree() const { return m_BehaviorTree; }
	FORCEINLINE UBlackboardData* GetBlackboard() const { return m_Blackboard; }
	FORCEINLINE UBoxComponent* GetRightWeaponCollsiion() const { return m_RightWeaponCollision; }
	FORCEINLINE UParticleSystem* GetHitParticle() const { return m_HitParticle; }
};