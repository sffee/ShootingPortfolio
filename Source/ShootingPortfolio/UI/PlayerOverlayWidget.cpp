#include "PlayerOverlayWidget.h"

#include "HPStaminaWidget.h"
#include "AmmoWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void UPlayerOverlayWidget::SetHP(float _CurHP, float _MaxHP)
{
	if (HPStaminaWidget == nullptr)
		return;

	float HPPercent = _CurHP / _MaxHP;
	HPStaminaWidget->HPBar->SetPercent(HPPercent);

	FString HPText = FString::Printf(TEXT("%d / %d"), (int32)_CurHP, (int32)_MaxHP);
	HPStaminaWidget->HPBarText->SetText(FText::FromString(HPText));
}

void UPlayerOverlayWidget::SetStamina(float _CurStamina, float _MaxStamina)
{
	if (HPStaminaWidget == nullptr)
		return;

	float StaminaPercent = _CurStamina / _MaxStamina;
	HPStaminaWidget->StaminaBar->SetPercent(StaminaPercent);

	FString StaminaText = FString::Printf(TEXT("%d / %d"), (int32)_CurStamina, (int32)_MaxStamina);
	HPStaminaWidget->StaminaBarText->SetText(FText::FromString(StaminaText));
}

void UPlayerOverlayWidget::SetWeaponName(FString _Name)
{
	if (AmmoWidget == nullptr)
		return;

	AmmoWidget->ItemName->SetText(FText::FromString(_Name));
}

void UPlayerOverlayWidget::SetAmmo(float _Ammo, float _Capacity)
{
	if (AmmoWidget == nullptr)
		return;

	AmmoWidget->Ammo->SetText(FText::AsNumber(_Ammo));
	AmmoWidget->Capacity->SetText(FText::AsNumber(_Capacity));
}