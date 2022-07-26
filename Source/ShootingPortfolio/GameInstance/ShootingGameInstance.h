#pragma once

#include "ShootingPortfolio/global.h"

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "ShootingGameInstance.generated.h"

UCLASS()
class SHOOTINGPORTFOLIO_API UShootingGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
private:
	UDataTable* m_CrosshairDataTable;

public:
	UShootingGameInstance();

public:
	FCrosshairTexture* GetCrosshairTexture(FName _TypeName);
};
