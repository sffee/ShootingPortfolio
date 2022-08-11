#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SpawnMonsterData.generated.h"

UCLASS()
class SHOOTINGPORTFOLIO_API USpawnMonsterData : public UObject
{
	GENERATED_BODY()

private:
	FName m_MonsterName;

	int32 m_SpawnCount;

public:
	FORCEINLINE void SetMonsterName(FName _Name) { m_MonsterName = _Name; }
	FORCEINLINE void SetSpawnCount(int32 _Count) { m_SpawnCount = _Count; }

public:
	FORCEINLINE FName GetMonsterName() const { return m_MonsterName; }
	FORCEINLINE int32 GetSpawnCount() const { return m_SpawnCount; }
};
