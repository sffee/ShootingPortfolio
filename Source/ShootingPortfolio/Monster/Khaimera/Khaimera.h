#pragma once

#include "CoreMinimal.h"
#include "../Monster.h"
#include "Khaimera.generated.h"

class AWarningMark;

UCLASS()
class SHOOTINGPORTFOLIO_API AKhaimera : public AMonster
{
	GENERATED_BODY()
	
private:
	UPROPERTY()
	TSubclassOf<AActor> m_WarningMarkClass;

	UPROPERTY()
	AWarningMark* m_WarningMark;
	
public:
	AKhaimera();

public:
	virtual void BeginPlay() override;

public:
	void CreateWarningMark(const FVector& _Location);
	void DestroyWarningMark();
};