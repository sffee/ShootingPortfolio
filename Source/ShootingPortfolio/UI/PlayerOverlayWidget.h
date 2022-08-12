#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerOverlayWidget.generated.h"

class UHPStaminaWidget;
class UAmmoWidget;
class UWeaponInventoryWidget;
class UTextBlock;
class UWaveMonsterCountWidget;

UCLASS()
class SHOOTINGPORTFOLIO_API UPlayerOverlayWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(meta = (BindWidget))
	UHPStaminaWidget* HPStaminaWidget;

	UPROPERTY(meta = (BindWidget))
	UAmmoWidget* AmmoWidget;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* WaveCountdownText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* WaveNumberText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* WaveStartCompleteText;

	UPROPERTY(meta = (BindWidget))
	UWeaponInventoryWidget* WeaponInventory;

	UPROPERTY(meta = (BindWidget))
	UWaveMonsterCountWidget* WaveMonsterCount;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	UWidgetAnimation* WaveStartAnimation;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	UWidgetAnimation* WaveCompleteAnimation;

protected:
	virtual void NativeConstruct() override;

public:
	void SetHP(float _CurHP, float _MaxHP);
	void SetStamina(float _CurStamina, float _MaxStamina);
	void SetWeaponName(FString _Name);
	void SetAmmo(int32 _Ammo, int32 _Capacity);
};