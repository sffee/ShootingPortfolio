#include "SpawnMonsterCircle.h"

#include "ShootingPortfolio/Monster/Monster.h"

ASpawnMonsterCircle::ASpawnMonsterCircle()
{
	m_Box = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	SetRootComponent(m_Box);

	m_Box->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	m_Box->SetBoxExtent(FVector(0.f, 0.f, 0.f));

	static ConstructorHelpers::FObjectFinder<UParticleSystem> SpawnStartParticle(TEXT("ParticleSystem'/Game/ParagonGideon/FX/Particles/Gideon/Abilities/Ultimate/FX/P_Gideon_Ultimate_Cast.P_Gideon_Ultimate_Cast'"));
	if (SpawnStartParticle.Succeeded())
		m_SpawnStartParticle = SpawnStartParticle.Object;

	static ConstructorHelpers::FObjectFinder<UParticleSystem> SpawnCompleteParticle(TEXT("ParticleSystem'/Game/ParagonMinions/FX/Particles/Minions/Prime_Helix/Abilities/SpecialAttack2/FX/P_PH_Shockwave.P_PH_Shockwave'"));
	if (SpawnCompleteParticle.Succeeded())
		m_SpawnCompleteParticle = SpawnCompleteParticle.Object;
}

void ASpawnMonsterCircle::BeginPlay()
{
	Super::BeginPlay();
	
	if (m_SpawnStartParticle)
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), m_SpawnStartParticle, GetActorLocation(), GetActorRotation(), m_SpawnStartParticleScale);
}

void ASpawnMonsterCircle::MonsterSpawn()
{
	if (m_SpawnCompleteParticle)
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), m_SpawnCompleteParticle, GetActorLocation() + m_SpawnCompleteParticleOffset, GetActorRotation(), m_SpawnCompleteParticleScale);

	if (m_SpawnMonsters.Num())
	{
		FActorSpawnParameters Param;
		Param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
		Param.bDeferConstruction = true;

		int32 Rand = FMath::RandRange(0, m_SpawnMonsters.Num() - 1);

		AMonster* Monster = GetWorld()->SpawnActor<AMonster>(m_SpawnMonsters[Rand], FVector::ZeroVector, FRotator::ZeroRotator, Param);

		FVector Start = GetActorLocation();
		FVector End = Start - FVector(0.f, 0.f, 1000.f);

		FHitResult HitResult;
		GetWorld()->LineTraceSingleByProfile(HitResult, Start, End, FName("LandScape"));
		FVector	SpawnLocation = HitResult.ImpactPoint + FVector(0.f, 0.f, Monster->GetCapsuleComponent()->GetScaledCapsuleHalfHeight());

		FTransform MonsterTransform = Monster->GetActorTransform();
		MonsterTransform.SetLocation(SpawnLocation);
		MonsterTransform.SetRotation(GetActorRotation().Quaternion());

		Monster->FinishSpawning(MonsterTransform);
	}
}