#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HPStaminaWidget.generated.h"

class UProgressBar;
class UTextBlock;

UCLASS()
class SHOOTINGPORTFOLIO_API UHPStaminaWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(meta = (BindWidget))
	UProgressBar* HPBar;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* HPBarText;

	UPROPERTY(meta = (BindWidget))
	UProgressBar* StaminaBar;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* StaminaBarText;
};
