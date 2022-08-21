#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "TitleGameMode.generated.h"

class UTitleOverlayWidget;
class USoundCue;

UCLASS()
class SHOOTINGPORTFOLIO_API ATitleGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
private:
	TSubclassOf<UUserWidget> m_TitleOverlayWidgetClass;

	UPROPERTY()
	UTitleOverlayWidget* m_TitleOverlayWidget;

private:
	UPROPERTY(EditAnywhere)
	USoundCue* m_GameStartSound;

	UPROPERTY(EditAnywhere)
	UAudioComponent* m_AudioComponent;
	
public:
	ATitleGameMode();

public:
	virtual void BeginPlay() override;

public:
	void StartLevelChange();

private:
	UFUNCTION()
	void FadeOutEnd();
};
