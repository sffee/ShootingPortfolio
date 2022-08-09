#pragma once

#include "ShootingPortfolio/global.h"

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WarningMark.generated.h"

UCLASS()
class SHOOTINGPORTFOLIO_API AWarningMark : public AActor
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	UDecalComponent* m_Decal;
	
public:	
	AWarningMark();
};