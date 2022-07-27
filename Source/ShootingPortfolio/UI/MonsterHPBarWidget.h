#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MonsterHPBarWidget.generated.h"

class UProgressBar;

UCLASS()
class SHOOTINGPORTFOLIO_API UMonsterHPBarWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(meta = (BindWidget))
	UProgressBar* HPBar;
};