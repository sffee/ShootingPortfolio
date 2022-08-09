#pragma once

#include "ShootingPortfolio/global.h"
#include "ShootingPortfolio/StructData.h"

#include <BehaviorTree/BehaviorTree.h>
#include <BehaviorTree/BlackboardComponent.h>

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Monster.generated.h"

DECLARE_DELEGATE_OneParam(FSpawnMonsterDieDelegate, UObject*)

class APlayerCharacter;

UCLASS()
class SHOOTINGPORTFOLIO_API AMonster : public ACharacter
{
	GENERATED_BODY()
	
public:
	FSpawnMonsterDieDelegate m_MonsterDieDelegate;

protected:
	UPROPERTY(EditAnywhere)
	UBoxComponent* m_RightWeaponCollision;

	UPROPERTY(EditAnywhere)
	UBoxComponent* m_LeftWeaponCollision;
	
protected:
	UPROPERTY(EditAnywhere, Category = Attack)
	UDataTable* m_AttackInfoDataTable;

	UPROPERTY(EditAnywhere, Category = Attack)
	TArray<FName> m_AttackSectionNameList;

	UPROPERTY(EditAnywhere, Category = Attack)
	UAnimMontage* m_AttackMontage;

	UPROPERTY(VisibleAnywhere, Category = Attack)
	int32 m_CurPlayAttackIndex;

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

public:
	void PlayMontage(UAnimMontage* _AnimMontage, FName _SectionName);
	void SpawnDamageText(const APlayerCharacter* _Player, float _Damage);

protected:
	UFUNCTION()
	virtual void ReceiveDamage(AActor* _DamagedActor, float _Damage, const UDamageType* _DamageType, class AController* _InstigatorController, AActor* _DamageCauser);

	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* _PrimitiveComponent, AActor* _OtherActor, UPrimitiveComponent* _OtherComp, int32 _OtherBodyIndex, bool _bFromSweep, const FHitResult& _SweepResult);

public:
	virtual void RightWeaponCollisionEnable();
	virtual void RightWeaponCollisionDisable();
	virtual void LeftWeaponCollisionEnable();
	virtual void LeftWeaponCollisionDisable();

public:
	void SetAttackSectionName(int32 _Index, FName _SectionName);

public:
	FORCEINLINE UBehaviorTree* GetBehaviorTree() const { return m_BehaviorTree; }
	FORCEINLINE UBlackboardData* GetBlackboard() const { return m_Blackboard; }
	FORCEINLINE UBoxComponent* GetRightWeaponCollsiion() const { return m_RightWeaponCollision; }
	FORCEINLINE UParticleSystem* GetHitParticle() const { return m_HitParticle; }
};