#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SniperRifleScopeWidget.generated.h"

UCLASS()
class SHOOTINGPORTFOLIO_API USniperRifleScopeWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidgetAnim), Transient)
	UWidgetAnimation* ZoomAnimation;
};
