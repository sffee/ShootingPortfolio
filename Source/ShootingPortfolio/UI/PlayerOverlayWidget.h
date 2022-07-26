#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerOverlayWidget.generated.h"

class UHPStaminaWidget;
class UAmmoWidget;

UCLASS()
class SHOOTINGPORTFOLIO_API UPlayerOverlayWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(meta = (BindWidget))
	UHPStaminaWidget* HPStaminaWidget;

	UPROPERTY(meta = (BindWidget))
	UAmmoWidget* AmmoWidget;

public:
	void SetHP(float _CurHP, float _MaxHP);
	void SetStamina(float _CurStamina, float _MaxStamina);
	void SetWeaponName(FString _Name);
	void SetAmmo(float _Ammo, float _Capacity);
};