#include "PickUp_Ammo.h"

#include "ShootingPortfolio/Player/PlayerCharacter.h"
#include "ShootingPortfolio/Player/PlayerCharacterController.h"

APickUp_Ammo::APickUp_Ammo()
	: m_AmmoAmountRate(0.3f)
	, m_RotationSpeed(90.f)
{
	static ConstructorHelpers::FObjectFinder<UStaticMesh> AmmoMesh(TEXT("StaticMesh'/Game/MilitaryWeapSilver/Pickups/Pistol_Pickup.Pistol_Pickup'"));
	if (AmmoMesh.Succeeded())
		m_StaticMesh->SetStaticMesh(AmmoMesh.Object);

	static ConstructorHelpers::FObjectFinder<USoundCue> PickUpSound(TEXT("SoundCue'/Game/MilitaryWeapSilver/Sound/Pistol/Cues/Pistol_ReloadInsert_Cue.Pistol_ReloadInsert_Cue'"));
	if (PickUpSound.Succeeded())
		m_PickUpSound = PickUpSound.Object;

	m_StaticMesh->SetRelativeLocation(FVector(0.f, 0.f, 34.f));
	m_StaticMesh->SetRelativeScale3D(FVector(3.f, 3.f, 3.f));
}

void APickUp_Ammo::RotationStaticMesh(float _DeltaTime)
{
	if (m_StaticMesh == nullptr)
		return;

	m_StaticMesh->AddRelativeRotation(FRotator(0.f, m_RotationSpeed * _DeltaTime, 0.f));
}

void APickUp_Ammo::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	RotationStaticMesh(DeltaTime);
}

void APickUp_Ammo::OnBeginOverlap(UPrimitiveComponent* _OverlappedComponent, AActor* _OtherActor, UPrimitiveComponent* _OtherComp, int32 _OtherBodyIndex, bool _bFromSweep, const FHitResult& _SweepResult)
{
	APlayerCharacter* Player = Cast<APlayerCharacter>(_OtherActor);
	if (Player == nullptr)
		return;

	APlayerCharacterController* PlayerController = Cast<APlayerCharacterController>(Player->GetController());
	if (PlayerController == nullptr)
		return;

	if (PlayerController->AmmoMapIsFull())
		return;

	PlayerController->AddAllAmmoRate(m_AmmoAmountRate);

	Super::OnBeginOverlap(_OverlappedComponent, _OtherActor, _OtherComp, _OtherBodyIndex, _bFromSweep, _SweepResult);
}