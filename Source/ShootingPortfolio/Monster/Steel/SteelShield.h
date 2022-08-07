#pragma once

#include "ShootingPortfolio/global.h"

#include "CoreMinimal.h"
#include "../NormalMonster.h"
#include "SteelShield.generated.h"

class UMonsterHPBarWidget;

UCLASS()
class SHOOTINGPORTFOLIO_API ASteelShield : public ANormalMonster
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditAnywhere)
	USkeletalMeshComponent* m_Mesh;

	UPROPERTY(EditAnywhere)
	UBoxComponent* m_CollisionBox;
	
public:	
	ASteelShield();

protected:
	virtual void BeginPlay() override;
	virtual void Destroyed() override;
};