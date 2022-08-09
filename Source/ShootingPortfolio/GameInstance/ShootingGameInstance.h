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
	UPROPERTY(EditAnywhere, Category = Crosshair)
	UDataTable* m_CrosshairDataTable;

	UPROPERTY(EditAnywhere, Category = Monster)
	TMap<FName, UDataTable*> m_MonsterAttackInfoDataTables;

public:
	UShootingGameInstance();

public:
	FCrosshairTexture* GetCrosshairTexture(const FName& _TypeName) const;
	UDataTable* GetMonsterAttackInfoDataTable(const FName& _MonsterName) const;
};
