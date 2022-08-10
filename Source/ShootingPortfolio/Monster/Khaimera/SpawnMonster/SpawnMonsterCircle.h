#pragma once

#include "ShootingPortfolio/global.h"

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpawnMonsterCircle.generated.h"

class AMonster;

UCLASS()
class SHOOTINGPORTFOLIO_API ASpawnMonsterCircle : public AActor
{
	GENERATED_BODY()
	
private:
	UPROPERTY()
	UBoxComponent* m_Box;
	
private:
	UPROPERTY(EditAnywhere, Category = Particle, meta = (AllowPrivateAccess = "true"))
	UParticleSystem* m_SpawnStartParticle;

	UPROPERTY(EditAnywhere, Category = Particle, meta = (AllowPrivateAccess = "true"))
	UParticleSystem* m_SpawnCompleteParticle;

	UPROPERTY(EditAnywhere, Category = Monster, meta = (AllowPrivateAccess = "true"))
	TArray<TSubclassOf<AMonster>> m_SpawnMonsters;

	UPROPERTY(EditAnywhere, Category = Particle, meta = (AllowPrivateAccess = "true"))
	FVector m_SpawnStartParticleScale;

	UPROPERTY(EditAnywhere, Category = Particle, meta = (AllowPrivateAccess = "true"))
	FVector m_SpawnCompleteParticleScale;

	UPROPERTY(EditAnywhere, Category = Particle, meta = (AllowPrivateAccess = "true"))
	FVector m_SpawnCompleteParticleOffset;
	
public:	
	ASpawnMonsterCircle();

protected:
	virtual void BeginPlay() override;

public:
	void MonsterSpawn();
};
