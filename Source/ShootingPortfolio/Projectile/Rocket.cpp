#include "Rocket.h"

#include "NiagaraSystem.h"

ARocket::ARocket()
{	
	static ConstructorHelpers::FObjectFinder<UStaticMesh> RocketMesh(TEXT("StaticMesh'/Game/MilitaryWeapSilver/Weapons/RocketLauncherA_Ammo.RocketLauncherA_Ammo'"));
	if (RocketMesh.Succeeded())
		m_Mesh->SetStaticMesh(RocketMesh.Object);

	static ConstructorHelpers::FObjectFinder<UParticleSystem> ImpactParticle(TEXT("ParticleSystem'/Game/MilitaryWeapSilver/FX/P_RocketLauncher_Explosion_01.P_RocketLauncher_Explosion_01'"));
	if (ImpactParticle.Succeeded())
		m_ImpactParticle = ImpactParticle.Object;

	static ConstructorHelpers::FObjectFinder<USoundCue> ImpactSound(TEXT("SoundCue'/Game/MilitaryWeapSilver/Sound/RocketLauncher/Cues/RocketLauncher_Explosion_Cue.RocketLauncher_Explosion_Cue'"));
	if (ImpactSound.Succeeded())
		m_ImpactSound = ImpactSound.Object;

	static ConstructorHelpers::FObjectFinder<UNiagaraSystem> TrailSystem(TEXT("NiagaraSystem'/Game/Game/Asset/FX/RocketTrail/NS_TrailSmoke.NS_TrailSmoke'"));
	if (TrailSystem.Succeeded())
		m_TrailSystem = TrailSystem.Object;

	m_ProjectileMovementComponent->InitialSpeed = 1500.f;
	m_ProjectileMovementComponent->MaxSpeed = 1500.f;
	m_ProjectileMovementComponent->ProjectileGravityScale = 0.f;

	m_DamageRadius = 500.f;
	m_LifeSpan = 5.f;
}

void ARocket::BeginPlay()
{
	Super::BeginPlay();

	SpawnTrailSystem();
}

void ARocket::Explosion()
{
	APawn* InstigatorPawn = GetInstigator();
	if (InstigatorPawn == nullptr)
		return;

	AController* Controller = InstigatorPawn->GetController();
	if (Controller == nullptr)
		return;

	AActor* Player = Cast<AActor>(GetOwner());
	if (Player == nullptr)
		return;

	TArray<AActor*> IgnoreActor;
	IgnoreActor.Add(Player);

	UGameplayStatics::ApplyRadialDamage(
		this,
		m_Damage,
		GetActorLocation(),
		m_DamageRadius,
		UDamageType::StaticClass(),
		IgnoreActor,
		this,
		Controller,
		true
	);

	DrawDebugSphere(GetWorld(), GetActorLocation(), m_DamageRadius, 20, FColor::Red, false, 5.f);
}

void ARocket::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	Super::OnHit(HitComp, OtherActor, OtherComp, NormalImpulse, Hit);

	Destroy();
}

void ARocket::Destroyed()
{
	Super::Destroyed();

	Explosion();
}
