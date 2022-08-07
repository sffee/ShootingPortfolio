#pragma once

#include "CoreMinimal.h"
#include "../NormalMonster.h"
#include "Minions.generated.h"

UCLASS()
class SHOOTINGPORTFOLIO_API AMinions : public ANormalMonster
{
	GENERATED_BODY()
	
private:
	

public:
	AMinions();
	
public:
	virtual void BeginPlay() override;
	virtual void Destroyed() override;
};
