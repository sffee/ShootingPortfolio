#include "WeaponSlotWidget.h"

#include "Components/Image.h"
#include "Components/TextBlock.h"

void UWeaponSlotWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (BackgroundImage)
		BackgroundImage->SetVisibility(ESlateVisibility::Hidden);

	if (WeaponIconImage)
		WeaponIconImage->SetVisibility(ESlateVisibility::Hidden);

	if (AmmoText)
		AmmoText->SetVisibility(ESlateVisibility::Hidden);
}