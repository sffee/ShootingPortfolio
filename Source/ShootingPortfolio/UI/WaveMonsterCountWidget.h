#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WaveMonsterCountWidget.generated.h"

class UListView;

UCLASS()
class SHOOTINGPORTFOLIO_API UWaveMonsterCountWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(meta = (BindWidget))
	UListView* MonsterList;
};
