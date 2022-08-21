#include "PickUp_HP.h"

#include "ShootingPortfolio/Player/PlayerCharacter.h"
#include "ShootingPortfolio/Player/PlayerCharacterController.h"

APickUp_HP::APickUp_HP()
	: m_HealAmount(100.f)
{
	static ConstructorHelpers::FObjectFinder<UNiagaraSystem> HP(TEXT("NiagaraSystem'/Game/sA_PickupSet_1/Fx/NiagaraSystems/NS_Pickup_3.NS_Pickup_3'"));
	if (HP.Succeeded())
		m_NiagaraComponent->SetAsset(HP.Object);

	static ConstructorHelpers::FObjectFinder<UNiagaraSystem> PickUp(TEXT("NiagaraSystem'/Game/sA_PickupSet_1/Fx/NiagaraSystems/NS_Healing_1.NS_Healing_1'"));
	if (PickUp.Succeeded())
		m_PickUpNiagaraSystem = PickUp.Object;

	static ConstructorHelpers::FObjectFinder<USoundCue> PickUpSound(TEXT("SoundCue'/Game/Interface_And_Item_Sounds/Cues/Coins_10_Cue.Coins_10_Cue'"));
	if (PickUpSound.Succeeded())
		m_PickUpSound = PickUpSound.Object;

	m_NiagaraComponent->SetRelativeLocation(FVector(0.f, 0.f, 17.f));

}

void APickUp_HP::OnBeginOverlap(UPrimitiveComponent* _OverlappedComponent, AActor* _OtherActor, UPrimitiveComponent* _OtherComp, int32 _OtherBodyIndex, bool _bFromSweep, const FHitResult& _SweepResult)
{
	APlayerCharacter* Player = Cast<APlayerCharacter>(_OtherActor);
	if (Player == nullptr)
		return;

	APlayerCharacterController* PlayerController = Cast<APlayerCharacterController>(Player->GetController());
	if (PlayerController == nullptr)
		return;

	if (PlayerController->HPIsFull())
		return;

	PlayerController->AddHP(m_HealAmount);

	Super::OnBeginOverlap(_OverlappedComponent, _OtherActor, _OtherComp, _OtherBodyIndex, _bFromSweep, _SweepResult);
}