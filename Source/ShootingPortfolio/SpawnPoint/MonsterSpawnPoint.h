#pragma once

#include "ShootingPortfolio/global.h"

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MonsterSpawnPoint.generated.h"

class UArrowComponent;

UCLASS()
class SHOOTINGPORTFOLIO_API AMonsterSpawnPoint : public AActor
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditAnywhere)
	UBoxComponent* m_Box;

	UPROPERTY(EditAnywhere)
	UArrowComponent* m_ArrowComponent;

	UPROPERTY(EditAnywhere)
	int32 m_Index;
	
public:	
	AMonsterSpawnPoint();

public:
	FORCEINLINE UBoxComponent* GetBoxComponent() const { return m_Box; }
	FORCEINLINE int32 GetIndex() const { return m_Index; }
};
