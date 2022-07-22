#include "ShootingGameMode.h"

AShootingGameMode::AShootingGameMode()
{
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawn(TEXT("Blueprint'/Game/Game/Blueprints/Player/BP_PlayerCharacter.BP_PlayerCharacter_C'"));
	if (PlayerPawn.Succeeded())
		DefaultPawnClass = PlayerPawn.Class;
}
