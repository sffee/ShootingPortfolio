#pragma once

#include "ShootingPortfolio/global.h"

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DamageTextActor.generated.h"

class UWidgetComponent;
class UDamageTextWidget;

UCLASS()
class SHOOTINGPORTFOLIO_API ADamageTextActor : public AActor
{
	GENERATED_BODY()
	
private:
	UPROPERTY()
	UWidgetComponent* m_DamageTextWidgetComponent;

	UPROPERTY()
	UDamageTextWidget* m_DamageTextWidget;

	UPROPERTY()
	TSubclassOf<UUserWidget> m_DamageTextWidgetClass;
	
public:	
	ADamageTextActor();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

private:
	void UpdateWidgetLocation();

public:
	void SetData(int32 _Damage, const FLinearColor& _Color);
	void DestroyTimerEnd();
};
