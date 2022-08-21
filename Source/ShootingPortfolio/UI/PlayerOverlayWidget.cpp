#include "PlayerOverlayWidget.h"

#include "HPStaminaWidget.h"
#include "AmmoWidget.h"
#include "WaveMonsterCountWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

#include <string>

void UPlayerOverlayWidget::NativeConstruct()
{
	Super::NativeConstruct();

	WaveCountdownText->SetVisibility(ESlateVisibility::Hidden);
	WaveNumberText->SetVisibility(ESlateVisibility::Hidden);
	WaveStartCompleteText->SetVisibility(ESlateVisibility::Hidden);
	WaveMonsterCount->SetVisibility(ESlateVisibility::Hidden);
	SkipWaveText->SetVisibility(ESlateVisibility::Hidden);
}

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

void UPlayerOverlayWidget::SetAmmo(int32 _Ammo, int32 _Capacity)
{
	if (AmmoWidget == nullptr)
		return;

	AmmoWidget->Ammo->SetText(FText::AsNumber(_Ammo));

	if (0 <= _Capacity)
	{
		AmmoWidget->Capacity->SetText(FText::AsNumber(_Capacity));

		FSlateFontInfo FontInfo;
		FontInfo.FontObject = LoadObject<UObject>(NULL, TEXT("Font'/Game/Game/Asset/Fonts/headhumancond_Font.headhumancond_Font'"));
		FontInfo.OutlineSettings = FFontOutlineSettings(1.f);
		FontInfo.Size = 30;
		AmmoWidget->Capacity->SetFont(FontInfo);
	}
	else
	{
		const TCHAR* Txt = TEXT("∞");
		AmmoWidget->Capacity->SetText(FText::FromString(Txt));

		FSlateFontInfo FontInfo;
		FontInfo.FontObject = LoadObject<UObject>(NULL, TEXT("Font'/Game/Game/Asset/Fonts/HeirofLightRegular_Font.HeirofLightRegular_Font'"));
		FontInfo.OutlineSettings = FFontOutlineSettings(1.f);
		FontInfo.Size = 30;
		AmmoWidget->Capacity->SetFont(FontInfo);
	}
}