#include "ShootingGameMode.h"

#include "ShootingPortfolio/Player/PlayerCharacterController.h"
#include "ShootingPortfolio/UI/ShootingHUD.h"

AShootingGameMode::AShootingGameMode()
{
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawn(TEXT("Blueprint'/Game/Game/Blueprints/Player/BP_PlayerCharacter.BP_PlayerCharacter_C'"));
	if (PlayerPawn.Succeeded())
		DefaultPawnClass = PlayerPawn.Class;

	static ConstructorHelpers::FClassFinder<APlayerController> PlayerController(TEXT("Blueprint'/Game/Game/Blueprints/Player/BP_PlayerCharacterController.BP_PlayerCharacterController_C'"));
	if (PlayerController.Succeeded())
		PlayerControllerClass = PlayerController.Class;

	HUDClass = AShootingHUD::StaticClass();
}