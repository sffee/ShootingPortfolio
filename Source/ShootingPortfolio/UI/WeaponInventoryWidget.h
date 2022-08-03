#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WeaponInventoryWidget.generated.h"

class UWeaponSlotWidget;

UCLASS()
class SHOOTINGPORTFOLIO_API UWeaponInventoryWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(meta = (BindWidget))
	UWeaponSlotWidget* WeaponSlot0;

	UPROPERTY(meta = (BindWidget))
	UWeaponSlotWidget* WeaponSlot1;

	UPROPERTY(meta = (BindWidget))
	UWeaponSlotWidget* WeaponSlot2;

	UPROPERTY(meta = (BindWidget))
	UWeaponSlotWidget* WeaponSlot3;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	UWidgetAnimation* WeaponSlot0Animation;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	UWidgetAnimation* WeaponSlot1Animation;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	UWidgetAnimation* WeaponSlot2Animation;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	UWidgetAnimation* WeaponSlot3Animation;
};
