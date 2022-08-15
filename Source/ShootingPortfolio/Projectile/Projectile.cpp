#include "Projectile.h"

#include "NiagaraFunctionLibrary.h"
#include "NiagaraSystem.h"
#include "NiagaraComponent.h"

AProjectile::AProjectile()
	: m_LifeSpan(0.f)
{
	m_CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	SetRootComponent(m_CollisionBox);
	m_CollisionBox->SetCollisionProfileName(TEXT("PlayerProjectile"));

	m_Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	m_Mesh->SetupAttachment(m_CollisionBox);

	m_ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
}

void AProjectile::BeginPlay()
{
	Super::BeginPlay();
	
	SetLifeSpan(m_LifeSpan);

	SpawnTrailSystem();

	m_CollisionBox->OnComponentHit.AddDynamic(this, &AProjectile::OnHit);
}

void AProjectile::SpawnTrailSystem()
{
	if (m_TrailSystem == nullptr)
		return;

	m_TrailSystemComponent = UNiagaraFunctionLibrary::SpawnSystemAttached(
		m_TrailSystem,
		GetRootComponent(),
		FName(),
		GetActorLocation(),
		GetActorRotation(),
		EAttachLocation::KeepWorldPosition,
		false
	);
}

void AProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	
}

void AProjectile::Destroyed()
{
	Super::Destroyed();

	if (m_ImpactParticle)
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), m_ImpactParticle, GetActorTransform());

	if (m_ImpactSound)
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), m_ImpactSound, GetActorLocation());
}