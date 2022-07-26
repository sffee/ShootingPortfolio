#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AmmoWidget.generated.h"

class UTextBlock;

UCLASS()
class SHOOTINGPORTFOLIO_API UAmmoWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* ItemName;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* Ammo;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* Capacity;
};