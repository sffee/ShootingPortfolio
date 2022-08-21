#include "TitleGameMode.h"

#include "ShootingPortfolio/UI/TitleOverlayWidget.h"
#include "Sound/SoundCue.h"

ATitleGameMode::ATitleGameMode()
{
	m_AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComponent"));

	static ConstructorHelpers::FClassFinder<UUserWidget> TitleOverlayWidget(TEXT("WidgetBlueprint'/Game/Game/Blueprints/UI/WBP_TitleOverlay.WBP_TitleOverlay_C'"));
	if (TitleOverlayWidget.Succeeded())
		m_TitleOverlayWidgetClass = TitleOverlayWidget.Class;

	static ConstructorHelpers::FObjectFinder<USoundCue> GameStartSound(TEXT("SoundCue'/Game/Game/Asset/Sound/Title/GameStart_Cue.GameStart_Cue'"));
	if (GameStartSound.Succeeded())
		m_GameStartSound = GameStartSound.Object;

	static ConstructorHelpers::FObjectFinder<USoundCue> BGM(TEXT("SoundCue'/Game/Game/Asset/Sound/BGM/TitleLevelBGM_Cue.TitleLevelBGM_Cue'"));
	if (BGM.Succeeded())
		m_AudioComponent->SetSound(BGM.Object);
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

	if (m_AudioComponent)
		m_AudioComponent->Play();
}

void ATitleGameMode::StartLevelChange()
{
	APlayerCameraManager* PlayerCameraManager = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0);
	if (PlayerCameraManager)
		PlayerCameraManager->StartCameraFade(0.f, 1.f, 3.f, FLinearColor::Black, false, true);

	m_AudioComponent->FadeOut(3.f, 0.f);

	if (m_GameStartSound)
		UGameplayStatics::PlaySound2D(GetWorld(), m_GameStartSound);

	if (m_TitleOverlayWidget)
		m_TitleOverlayWidget->PlayAnimation(m_TitleOverlayWidget->FadeInAnimation);
		
	FTimerHandle Timer;
	GetWorldTimerManager().SetTimer(Timer, this, &ATitleGameMode::FadeOutEnd, 3.5f);
}

void ATitleGameMode::FadeOutEnd()
{
	UGameplayStatics::OpenLevel(GetWorld(), TEXT("PlayLevel"));
}