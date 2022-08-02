#pragma once

#include "ShootingPortfolio/global.h"

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "ShootingHUD.generated.h"

class UPlayerOverlayWidget;
class APlayerCharacterController;

UCLASS()
class SHOOTINGPORTFOLIO_API AShootingHUD : public AHUD
{
	GENERATED_BODY()
	
	friend APlayerCharacterController;
	
private:
	TSubclassOf<UUserWidget> m_PlayerOverlayWidgetClass;
	UPlayerOverlayWidget* m_PlayerOverlayWidget;
	
private:
	UPROPERTY(VisibleAnywhere)
	FCrosshairTexture m_CrosshairTexture;

	UPROPERTY(VisibleAnywhere)
	float m_CrosshairSpread;

	UPROPERTY(VisibleAnywhere)
	FLinearColor m_CrosshairColor;
	
public:
	AShootingHUD();

public:
	virtual void BeginPlay() override;
	virtual void DrawHUD() override;

private:
	void DrawCrosshair(UTexture2D* _Texture, const FVector2D& _ViewportCenter, const FVector2D& _Spread);

public:
	void SetHP(float _CurHP, float _MaxHP);
	void SetStamina(float _CurStamina, float _MaxStamina);
	void SetWeaponName(FString _Name);
	void SetAmmo(float _Ammo, float _Capacity);

public:
	void SetCrosshairType(FName _TypeName);
	FORCEINLINE void SetCrosshairSpread(const float _Spread) { m_CrosshairSpread = _Spread; }
	FORCEINLINE void SetCrosshairColor(const FLinearColor& _Color) { m_CrosshairColor = _Color; }

public:
	FORCEINLINE UPlayerOverlayWidget* GetPlayerOverlayWidget() const { return m_PlayerOverlayWidget; }
};