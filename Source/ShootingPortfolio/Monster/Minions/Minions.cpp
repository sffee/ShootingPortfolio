#include "Minions.h"

#include "ShootingPortfolio/Player/PlayerCharacter.h"

AMinions::AMinions()
	: m_AttackDamage(10.f)
{
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> MinionsMesh(TEXT("SkeletalMesh'/Game/ParagonMinions/Characters/Minions/Down_Minions/Meshes/Minion_Lane_Melee_Core_Dawn.Minion_Lane_Melee_Core_Dawn'"));
	if (MinionsMesh.Succeeded())
		GetMesh()->SetSkeletalMesh(MinionsMesh.Object);

	static ConstructorHelpers::FObjectFinder<UParticleSystem> DeathParticle(TEXT("ParticleSystem'/Game/ParagonMinions/FX/Particles/Minions/Minion_melee/FX/Death/P_RangedMinion_Chunks3.P_RangedMinion_Chunks3'"));
	if (DeathParticle.Succeeded())
		m_DeathParticle = DeathParticle.Object;

	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BehaviorTree(TEXT("BehaviorTree'/Game/Game/Blueprints/Monster/Minions/BT_Minions.BT_Minions'"));
	if (BehaviorTree.Succeeded())
		m_BehaviorTree = BehaviorTree.Object;

	static ConstructorHelpers::FObjectFinder<UAnimMontage> AttackMontage(TEXT("AnimMontage'/Game/Game/Blueprints/Monster/Minions/Animation/AttackMontage.AttackMontage'"));
	if (AttackMontage.Succeeded())
		m_AttackMontage = AttackMontage.Object;

	static ConstructorHelpers::FObjectFinder<UParticleSystem> HitParticle(TEXT("ParticleSystem'/Game/ParagonMinions/FX/Particles/Minions/Shared/P_Minion_Melee_Impact.P_Minion_Melee_Impact'"));
	if (HitParticle.Succeeded())
		m_HitParticle = HitParticle.Object;

	GetMesh()->SetWorldLocation(FVector(0.f, 0.f, -88.f));
	GetMesh()->SetWorldRotation(FRotator(0.f, -90.f, 0.f));

	if (m_HPBarWidgetComponent)
		m_HPBarWidgetComponent->SetRelativeLocation(FVector(0.f, 0.f, 180.f));

	m_Status.MaxHP = 100;

	m_AttackSectionNameList.Add(FName("Attack"));
}

void AMinions::BeginPlay()
{
	Super::BeginPlay();


}

void AMinions::Destroyed()
{
	Super::Destroyed();

	if (m_DeathParticle)
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), m_DeathParticle, GetActorTransform());
}

void AMinions::OnBeginOverlap(UPrimitiveComponent* _PrimitiveComponent, AActor* _OtherActor, UPrimitiveComponent* _OtherComp, int32 _OtherBodyIndex, bool _bFromSweep, const FHitResult& _SweepResult)
{
	Super::OnBeginOverlap(_PrimitiveComponent, _OtherActor, _OtherComp, _OtherBodyIndex, _bFromSweep, _SweepResult);

	APlayerCharacter* Player = Cast<APlayerCharacter>(_OtherActor);
	if (Player && Controller)
		UGameplayStatics::ApplyDamage(Player, m_AttackDamage, Controller, this, UDamageType::StaticClass());
}
