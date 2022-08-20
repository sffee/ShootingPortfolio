#include "TitleOverlayWidget.h"

#include <Components/Button.h>
#include "ShootingPortfolio/GameMode/TitleGameMode.h"

void UTitleOverlayWidget::NativeConstruct()
{
	if (Start)
		Start->OnClicked.AddDynamic(this, &UTitleOverlayWidget::OnStartButtonClicked);

	if (End)
		End->OnClicked.AddDynamic(this, &UTitleOverlayWidget::OnEndButtonClicked);
}

void UTitleOverlayWidget::OnStartButtonClicked()
{
	ATitleGameMode* GameMode = Cast<ATitleGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (GameMode == nullptr)
		return;
	
	GameMode->StartLevelChange();
}

void UTitleOverlayWidget::OnEndButtonClicked()
{
	FGenericPlatformMisc::RequestExit(false);
}