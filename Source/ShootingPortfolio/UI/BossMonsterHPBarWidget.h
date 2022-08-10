#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BossMonsterHPBarWidget.generated.h"

class UProgressBar;
class UTextBlock;

UCLASS()
class SHOOTINGPORTFOLIO_API UBossMonsterHPBarWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* BossMonsterName;

	UPROPERTY(meta = (BindWidget))
	UProgressBar* HPBar;

	UPROPERTY(meta = (BindWidget))
	UProgressBar* ShieldBar;
};