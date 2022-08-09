#pragma once

#include "CoreMinimal.h"
#include "../Monster.h"
#include "Khaimera.generated.h"

UCLASS()
class SHOOTINGPORTFOLIO_API AKhaimera : public AMonster
{
	GENERATED_BODY()
	
public:
	AKhaimera();

public:
	virtual void BeginPlay() override;
};