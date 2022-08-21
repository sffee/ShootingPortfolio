#include "PickUp.h"

#include "NiagaraComponent.h"

APickUp::APickUp()
	: m_IsSpawned(true)
{
	PrimaryActorTick.bCanEverTick = true;

	m_GroundMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("GroundMesh"));
	SetRootComponent(m_GroundMesh);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> GroundMesh(TEXT("StaticMesh'/Game/Game/Asset/Mesh/PickUp/PickUp_Ground.PickUp_Ground'"));
	if (GroundMesh.Succeeded())
		m_GroundMesh->SetStaticMesh(GroundMesh.Object);

	m_NiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NiagaraComponent"));
	m_NiagaraComponent->SetupAttachment(m_GroundMesh);

	m_StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	m_StaticMesh->SetupAttachment(m_GroundMesh);

	m_CollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionSphere"));
	m_CollisionSphere->SetupAttachment(m_GroundMesh);
	m_CollisionSphere->SetSphereRadius(75.f);
	m_CollisionSphere->SetCollisionProfileName(TEXT("PickUp"));
}

void APickUp::SpawnItem()
{
	if (m_IsSpawned)
		return;

	if (m_NiagaraComponent)
		m_NiagaraComponent->SetVisibility(true);

	if (m_StaticMesh)
		m_StaticMesh->SetVisibility(true);

	m_CollisionSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

	m_IsSpawned = true;
}

void APickUp::UnSpawnItem()
{
	if (m_NiagaraComponent)
		m_NiagaraComponent->SetVisibility(false);

	if (m_StaticMesh)
		m_StaticMesh->SetVisibility(false);

	m_CollisionSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	m_IsSpawned = false;
}

void APickUp::BeginPlay()
{
	Super::BeginPlay();
	
	UnSpawnItem();

	m_CollisionSphere->OnComponentBeginOverlap.AddDynamic(this, &APickUp::OnBeginOverlap);
}

void APickUp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APickUp::OnBeginOverlap(UPrimitiveComponent* _OverlappedComponent, AActor* _OtherActor, UPrimitiveComponent* _OtherComp, int32 _OtherBodyIndex, bool _bFromSweep, const FHitResult& _SweepResult)
{
	UnSpawnItem();

	if (m_PickUpNiagaraSystem)
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), m_PickUpNiagaraSystem, m_NiagaraComponent->GetComponentLocation());

	if (m_PickUpSound)
		UGameplayStatics::PlaySound2D(GetWorld(), m_PickUpSound);
}