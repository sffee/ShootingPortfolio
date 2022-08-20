#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "TitleGameMode.generated.h"

class UTitleOverlayWidget;

class ULevelSequence;
class ALevelSequenceActor;
class ULevelSequencePlayer;

UCLASS()
class SHOOTINGPORTFOLIO_API ATitleGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditAnywhere, Category = LevelSequence)
	ULevelSequence* m_FadeInLevelSequence;

	UPROPERTY()
	ALevelSequenceActor* m_SequenceActor;

	UPROPERTY()
	ULevelSequencePlayer* m_SequencePlayer;
	
private:
	TSubclassOf<UUserWidget> m_TitleOverlayWidgetClass;

	UPROPERTY()
	UTitleOverlayWidget* m_TitleOverlayWidget;
	
public:
	ATitleGameMode();

public:
	virtual void BeginPlay() override;

public:
	void StartLevelChange();

private:
	UFUNCTION()
	void FadeInSequenceEnd();
};
