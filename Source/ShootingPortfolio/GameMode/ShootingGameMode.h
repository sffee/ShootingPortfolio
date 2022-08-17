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

class UPlayerOverlayWidget;
class APlayerCharacterController;
class ULevelSequence;
class ULevelSequencePlayer;
class ALevelSequenceActor;

UCLASS()
class SHOOTINGPORTFOLIO_API AShootingGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditAnywhere, Category = LevelSequence)
	ULevelSequence* m_BossAppearLevelSequence;

	UPROPERTY()
	ALevelSequenceActor* m_SequenceActor;

	UPROPERTY()
	ULevelSequencePlayer* m_SequencePlayer;
	
private:
	UPlayerOverlayWidget* m_PlayerOverlayWidget;
	APlayerCharacterController* m_PlayerController;
	
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

	UPROPERTY(VisibleAnywhere, Category = Wave)
	EWaveState m_WaveState;

private:
	TMap<int32, FMonsterSpawnPointData> m_SpawnPointMap;

	TArray<FTimerHandle> m_SpawnTimers;
	TArray<FMonsterSpawnData> m_SpawnDatas;
	TMap<UObject*, int32> m_SpawnCountMap;
	TMap<UObject*, int32> m_AliveMonsterMap;

private:
	FTimerHandle m_GameStartTimer;
	
public:
	AShootingGameMode();

public:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

private:
	void GameStartTimerEnd();

private:
	void StartWaveCountdown();
	void StartWaveComplete();

private:
	void InitSpawnPointMap();
	void ResetData();

public:
	void SpawnStart();
	void BossWave();

	UFUNCTION()
	void BossSequenceEnd();

private:
	void SpawnMonsterProcess(TSubclassOf<AMonster> _Monster, int32 _Index);
	void SpawnMonster(TSubclassOf<AMonster> _Monster, const FMonsterSpawnPointData& _SpawnPointData);

private:
	void Delegate_MonsterDie(UObject* _Monster);

public:
	FORCEINLINE void SetWaveState(EWaveState _State) { m_WaveState = _State; }

public:
	FORCEINLINE int32 GetWaveNumber() const { return m_Wave; }
	FORCEINLINE bool IsBossWave() const { return m_Wave == m_BossWave; }
};