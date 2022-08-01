#include "DamageTextActor.h"

#include <Components/WidgetComponent.h>
#include "DamageTextWidget.h"
#include "Components/TextBlock.h"

ADamageTextActor::ADamageTextActor()
{
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FClassFinder<UUserWidget> DamageTextWidget(TEXT("WidgetBlueprint'/Game/Game/Blueprints/UI/WBP_DamageText.WBP_DamageText_C'"));
	if (DamageTextWidget.Succeeded())
		m_DamageTextWidgetClass = DamageTextWidget.Class;

	m_DamageTextWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("DamageTextWidget"));
	m_DamageTextWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);

	SetRootComponent(m_DamageTextWidgetComponent);
}

void ADamageTextActor::BeginPlay()
{
	Super::BeginPlay();

	if (m_DamageTextWidgetClass)
	{
		m_DamageTextWidget = CreateWidget<UDamageTextWidget>(GetWorld(), m_DamageTextWidgetClass);
		m_DamageTextWidget->AddToViewport();
	}

	FTimerHandle Timer;
	GetWorldTimerManager().SetTimer(Timer, this, &ADamageTextActor::DestroyTimerEnd, 1.f);
}

void ADamageTextActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdateWidgetLocation();
}

void ADamageTextActor::UpdateWidgetLocation()
{
	if (m_DamageTextWidget == nullptr)
		return;

	FVector2D ScreenLocation;
	UGameplayStatics::ProjectWorldToScreen(UGameplayStatics::GetPlayerController(this, 0), GetActorLocation(), ScreenLocation);

	m_DamageTextWidget->SetPositionInViewport(ScreenLocation);
}

void ADamageTextActor::SetData(int32 _Damage, const FLinearColor& _Color)
{
	if (m_DamageTextWidget == nullptr)
		return;

	m_DamageTextWidget->DamageText->SetText(FText::AsNumber(_Damage));
	m_DamageTextWidget->DamageText->SetColorAndOpacity(FSlateColor(_Color));

	m_DamageTextWidget->PlayAnimation(m_DamageTextWidget->TextAnimation);
}

void ADamageTextActor::DestroyTimerEnd()
{
	m_DamageTextWidget->RemoveFromParent();
	Destroy();
}