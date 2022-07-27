#include "Minions.h"

AMinions::AMinions()
{
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> MinionsMesh(TEXT("SkeletalMesh'/Game/ParagonMinions/Characters/Minions/Down_Minions/Meshes/Minion_Lane_Melee_Core_Dawn.Minion_Lane_Melee_Core_Dawn'"));
	if (MinionsMesh.Succeeded())
		GetMesh()->SetSkeletalMesh(MinionsMesh.Object);

	GetMesh()->SetWorldLocation(FVector(0.f, 0.f, -88.f));
	GetMesh()->SetWorldRotation(FRotator(0.f, -90.f, 0.f));

	if (m_HPBarWidgetComponent)
		m_HPBarWidgetComponent->SetRelativeLocation(FVector(0.f, 0.f, 180.f));

	m_Status.MaxHP = 100;
}

void AMinions::BeginPlay()
{
	Super::BeginPlay();


}