#include "BossDoor.h"

ABossDoor::ABossDoor()
	: m_Damage(5000.f)
	, m_Time(1.f)
{
	PrimaryActorTick.bCanEverTick = true;

	m_DestructibleComponent = CreateDefaultSubobject<UDestructibleComponent>(TEXT("DestructibleComponent"));
	SetRootComponent(m_DestructibleComponent);

	m_ImpactSphere = CreateDefaultSubobject<USphereComponent>(TEXT("ImpactSphere"));
	m_ImpactSphere->SetupAttachment(m_DestructibleComponent);

	static ConstructorHelpers::FObjectFinder<UDestructibleMesh> Mesh(TEXT("DestructibleMesh'/Game/Game/Asset/Mesh/Destructable/Dawn_Wall_Elaborate_Straight_A_DM.Dawn_Wall_Elaborate_Straight_A_DM'"));
	if (Mesh.Succeeded())
		m_DestructibleComponent->SetDestructibleMesh(Mesh.Object);

	//m_DestructibleComponent->
}

void ABossDoor::BeginPlay()
{
	Super::BeginPlay();
}

void ABossDoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABossDoor::DoorDestruction()
{
	//m_DestructibleComponent->AddRadialForce(m_ImpactSphere->GetComponentLocation(), m_ImpactSphere->GetScaledSphereRadius(), m_Damage, ERadialImpulseFalloff::RIF_Constant);// AddForceAtLocation(FVector(m_Damage, m_Damage, m_Damage), m_ImpactSphere->GetComponentLocation());
	UGameplayStatics::ApplyRadialDamage(this, m_Damage, m_ImpactSphere->GetComponentLocation(), m_ImpactSphere->GetScaledSphereRadius(), UDamageType::StaticClass(), TArray<AActor*>());
}
