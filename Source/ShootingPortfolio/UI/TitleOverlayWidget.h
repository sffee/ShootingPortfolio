#pragma once

#include "ShootingPortfolio/global.h"

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TitleOverlayWidget.generated.h"

class UButton;

UCLASS()
class SHOOTINGPORTFOLIO_API UTitleOverlayWidget : public UUserWidget
{
	GENERATED_BODY()
	
private:
	UPROPERTY(meta = (BindWidget))
	UButton* Start;

	UPROPERTY(meta = (BindWidget))
	UButton* End;

public:
	virtual void NativeConstruct() override;

public:
	UFUNCTION()
	void OnStartButtonClicked();

	UFUNCTION()
	void OnEndButtonClicked();
};