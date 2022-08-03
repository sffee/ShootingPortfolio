#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WeaponSlotWidget.generated.h"

class UImage;
class UTextBlock;

UCLASS()
class SHOOTINGPORTFOLIO_API UWeaponSlotWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(meta = (BindWidget))
	UImage* BackgroundImage;

	UPROPERTY(meta = (BindWidget))
	UImage* WeaponIconImage;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* AmmoText;

protected:
	virtual void NativeConstruct() override;
};
