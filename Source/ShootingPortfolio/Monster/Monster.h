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
	UPROPERTY(VisibleAnywhere)
	FName m_Name;

protected:
	UPROPERTY(EditAnywhere)
	UBoxComponent* m_RightWeaponCollision;

	UPROPERTY(EditAnywhere)
	UBoxComponent* m_LeftWeaponCollision;
	
protected:
	UPROPERTY(VisibleAnywhere, Category = Attack)
	TMap<FName, float> m_AttackCooltimes;

	UPROPERTY(EditAnywhere, Category = Attack)
	UAnimMontage* m_AttackMontage;

	UPROPERTY(VisibleAnywhere, Category = Attack)
	FName m_CurPlayAttackSectionName;

	bool m_IsAttacking;

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

private:
	void InitAttackInfoDataTable();

public:
	bool PlayAttackSection(const FName& _SectionName);

public:
	void PlayMontage(UAnimMontage* _AnimMontage, FName _SectionName);
	void SpawnDamageText(const APlayerCharacter* _Player, float _Damage);

protected:
	virtual float TakeDamage(float _DamageAmount, FDamageEvent const& _DamageEvent, AController* _EventInstigator, AActor* _DamageCauser) override;

	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* _PrimitiveComponent, AActor* _OtherActor, UPrimitiveComponent* _OtherComp, int32 _OtherBodyIndex, bool _bFromSweep, const FHitResult& _SweepResult);

	UFUNCTION()
	void OnAttackMontageEnded(UAnimMontage* _Montage, bool _bInterrupted);

public:
	virtual void RightWeaponCollisionEnable();
	virtual void RightWeaponCollisionDisable();
	virtual void LeftWeaponCollisionEnable();
	virtual void LeftWeaponCollisionDisable();

public:
	FORCEINLINE UBehaviorTree* GetBehaviorTree() const { return m_BehaviorTree; }
	FORCEINLINE UBlackboardData* GetBlackboard() const { return m_Blackboard; }
	FORCEINLINE UBoxComponent* GetRightWeaponCollsiion() const { return m_RightWeaponCollision; }
	FORCEINLINE UParticleSystem* GetHitParticle() const { return m_HitParticle; }
	FORCEINLINE bool IsAttacking() const { return m_IsAttacking; }
	FORCEINLINE UAnimMontage* GetAttackAnimMontage() const { return m_AttackMontage; }
	FORCEINLINE FName GetMonsterName() const { return m_Name; }
};