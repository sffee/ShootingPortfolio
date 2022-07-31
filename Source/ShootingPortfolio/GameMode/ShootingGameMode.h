#pragma once

#include "ShootingPortfolio/global.h"

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ShootingGameMode.generated.h"

class AMonsterSpawnPoint;

USTRUCT(BlueprintType)
struct FMonsterSpawnPointData
{
	GENERATED_BODY()
	
	UPROPERTY()
	AMonsterSpawnPoint* SpawnPoint;

	FVector Location;
	FRotator Rotation;
	FVector Scale;
};

UCLASS()
class SHOOTINGPORTFOLIO_API AShootingGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditAnywhere)
	UDataTable* m_WaveDataTable;

	UPROPERTY(VisibleAnywhere, Category = Wave)
	int32 m_Wave;

	UPROPERTY(EditAnywhere, Category = Wave)
	int32 m_Phase2Wave;

	UPROPERTY(EditAnywhere, Category = Wave)
	int32 m_BossWave;

	UPROPERTY(VisibleAnywhere, Category = Wave)
	int32 m_SpawnPointCount;

	UPROPERTY(VisibleAnywhere, Category = Wave)
	int32 m_SpawnCompleteMonsterCount;

	UPROPERTY(VisibleAnywhere, Category = Wave)
	int32 m_NeedSpawnMonsterCount;

	UPROPERTY(VisibleAnywhere, Category = Wave)
	int32 m_AliveMonsterCount;

	UPROPERTY(VisibleAnywhere, Category = Wave)
	bool m_SpawnComplete;
	
private:
	TMap<int32, FMonsterSpawnPointData> m_SpawnPointMap;

	TArray<FTimerHandle> m_SpawnTimers;
	TArray<FMonsterSpawnData> m_SpawnDatas;
	TMap<UObject*, int32> m_SpawnCountMap;
	TMap<UObject*, int32> m_AliveMonsterMap;
	
public:
	AShootingGameMode();

public:
	virtual void BeginPlay() override;
	virtual void Tick(float _DeltaTime) override;

private:
	void InitSpawnPointMap();
	void ResetData();

private:
	void SpawnStart();
	void SpawnMonster(TSubclassOf<AMonster> _Monster, int32 _Index);
	void SpawnMonster(TSubclassOf<AMonster> _Monster, const FMonsterSpawnPointData& _SpawnPointData);

private:
	void Delegate_MonsterDie(UObject* _Monster);
};