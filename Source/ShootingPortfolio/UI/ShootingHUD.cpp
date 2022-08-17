#include "ShootingHUD.h"

#include "ShootingPortfolio/GameInstance/ShootingGameInstance.h"
#include "PlayerOverlayWidget.h"

AShootingHUD::AShootingHUD()
	: m_CrosshairSpread(0.f)
	, m_CrosshairColor(FLinearColor::White)
	, m_DrawCrosshair(true)
{
	ConstructorHelpers::FClassFinder<UUserWidget> PlayerOverlayWidgetClass(TEXT("WidgetBlueprint'/Game/Game/Blueprints/UI/WBP_HUDOverlay.WBP_HUDOverlay_C'"));
	if (PlayerOverlayWidgetClass.Succeeded())
		m_PlayerOverlayWidgetClass = PlayerOverlayWidgetClass.Class;
}

void AShootingHUD::BeginPlay()
{
	Super::BeginPlay();

	if (m_PlayerOverlayWidgetClass)
	{
		m_PlayerOverlayWidget = CreateWidget<UPlayerOverlayWidget>(GetOwningPlayerController(), m_PlayerOverlayWidgetClass);
		m_PlayerOverlayWidget->AddToViewport();
	}
}

void AShootingHUD::DrawHUD()
{
	Super::DrawHUD();

	if (!GEngine || m_DrawCrosshair == false)
		return;

	FVector2D ViewportSize;
	GEngine->GameViewport->GetViewportSize(ViewportSize);
	const FVector2D ViewportCenter(ViewportSize.X * 0.5f, ViewportSize.Y * 0.5f);

	if (m_CrosshairTexture.CenterTexture)
	{
		FVector2D Spread(0.f, 0.f);
		DrawCrosshair(m_CrosshairTexture.CenterTexture, ViewportCenter, Spread);
	}

	if (m_CrosshairTexture.LeftTexture)
	{
		FVector2D Spread(-m_CrosshairSpread, 0.f);
		DrawCrosshair(m_CrosshairTexture.LeftTexture, ViewportCenter, Spread);
	}

	if (m_CrosshairTexture.RightTexture)
	{
		FVector2D Spread(m_CrosshairSpread, 0.f);
		DrawCrosshair(m_CrosshairTexture.RightTexture, ViewportCenter, Spread);
	}

	if (m_CrosshairTexture.TopTexture)
	{
		FVector2D Spread(0.f, -m_CrosshairSpread);
		DrawCrosshair(m_CrosshairTexture.TopTexture, ViewportCenter, Spread);
	}

	if (m_CrosshairTexture.BottomTexture)
	{
		FVector2D Spread(0.f, m_CrosshairSpread);
		DrawCrosshair(m_CrosshairTexture.BottomTexture, ViewportCenter, Spread);
	}
}

void AShootingHUD::DrawCrosshair(UTexture2D* _Texture, const FVector2D& _ViewportCenter, const FVector2D& _Spread)
{
	const float TextureWidth = _Texture->GetSizeX();
	const float TextureHeight = _Texture->GetSizeY();
	const FVector2D TextureDrawPoint(_ViewportCenter.X - (TextureWidth * 0.5f) + _Spread.X, _ViewportCenter.Y - (TextureHeight * 0.5f) + _Spread.Y);

	DrawTexture(
		_Texture,
		TextureDrawPoint.X,
		TextureDrawPoint.Y,
		TextureWidth,
		TextureHeight,
		0.f,
		0.f,
		1.f,
		1.f,
		m_CrosshairColor
	);
}

void AShootingHUD::SetHP(float _CurHP, float _MaxHP)
{
	if (m_PlayerOverlayWidget == nullptr)
		return;

	m_PlayerOverlayWidget->SetHP(_CurHP, _MaxHP);
}

void AShootingHUD::SetStamina(float _CurStamina, float _MaxStamina)
{
	if (m_PlayerOverlayWidget == nullptr)
		return;

	m_PlayerOverlayWidget->SetStamina(_CurStamina, _MaxStamina);
}

void AShootingHUD::SetWeaponName(FString _Name)
{
	if (m_PlayerOverlayWidget == nullptr)
		return;

	m_PlayerOverlayWidget->SetWeaponName(_Name);
}

void AShootingHUD::SetAmmo(int32 _Ammo, int32 _Capacity)
{
	if (m_PlayerOverlayWidget == nullptr)
		return;

	m_PlayerOverlayWidget->SetAmmo(_Ammo, _Capacity);
}

void AShootingHUD::SetHUDVisibility(bool _Set)
{
	if (m_PlayerOverlayWidget == nullptr)
		return;

	m_DrawCrosshair = _Set;
	
	if (_Set)
		m_PlayerOverlayWidget->SetVisibility(ESlateVisibility::Visible);
	else
		m_PlayerOverlayWidget->SetVisibility(ESlateVisibility::Hidden);
}

void AShootingHUD::SetCrosshairType(const ECrosshairType _Type)
{
	UShootingGameInstance* GameInstance = Cast<UShootingGameInstance>(UGameplayStatics::GetGameInstance(this));
	if (GameInstance == nullptr)
		return;

	m_CrosshairTexture = *GameInstance->GetCrosshairTexture(_Type);
}