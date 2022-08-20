#include "TitleGameMode.h"

#include "ShootingPortfolio/UI/TitleOverlayWidget.h"
#include "LevelSequence/Public/LevelSequenceActor.h"
#include "LevelSequence/Public/LevelSequence.h"
#include "LevelSequence/Public/LevelSequencePlayer.h"

ATitleGameMode::ATitleGameMode()
{
	static ConstructorHelpers::FClassFinder<UUserWidget> TitleOverlayWidget(TEXT("WidgetBlueprint'/Game/Game/Blueprints/UI/WBP_TitleOverlay.WBP_TitleOverlay_C'"));
	if (TitleOverlayWidget.Succeeded())
		m_TitleOverlayWidgetClass = TitleOverlayWidget.Class;

	static ConstructorHelpers::FObjectFinder<ULevelSequence> FadeInLevelSequence(TEXT("LevelSequence'/Game/Game/Blueprints/Sequencer/FadeInSequence.FadeInSequence'"));
	if (FadeInLevelSequence.Succeeded())
		m_FadeInLevelSequence = FadeInLevelSequence.Object;
}

void ATitleGameMode::BeginPlay()
{
	Super::BeginPlay();

	APlayerController* Controller = GetWorld()->GetFirstPlayerController();
	if (Controller)
	{
		FInputModeUIOnly Mode;

		Controller->SetInputMode(Mode);
		Controller->bShowMouseCursor = true;
	}

	if (m_TitleOverlayWidgetClass)
	{
		m_TitleOverlayWidget = CreateWidget<UTitleOverlayWidget>(GetWorld(), m_TitleOverlayWidgetClass);
		m_TitleOverlayWidget->AddToViewport();
	}
}

void ATitleGameMode::StartLevelChange()
{
	if (m_FadeInLevelSequence)
	{
		m_SequencePlayer = ULevelSequencePlayer::CreateLevelSequencePlayer(GetWorld(), m_FadeInLevelSequence, FMovieSceneSequencePlaybackSettings(), m_SequenceActor);
		m_SequencePlayer->OnFinished.AddDynamic(this, &ATitleGameMode::FadeInSequenceEnd);

		m_SequencePlayer->Play();
	}
}

void ATitleGameMode::FadeInSequenceEnd()
{
	UGameplayStatics::OpenLevel(GetWorld(), TEXT("PlayLevel"));
}