#include "PlayerCharacter.h"

APlayerCharacter::APlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> PlayerMesh(TEXT("SkeletalMesh'/Game/ParagonLtBelica/Characters/Heroes/Belica/Meshes/Belica.Belica'"));
	if (PlayerMesh.Succeeded())
		GetMesh()->SetSkeletalMesh(PlayerMesh.Object);
}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

