#include "MonsterSpawnPoint.h"

#include "Components/ArrowComponent.h"

AMonsterSpawnPoint::AMonsterSpawnPoint()
	: m_Index(0)
{
	m_Box = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	SetRootComponent(m_Box);

	m_ArrowComponent = CreateDefaultSubobject<UArrowComponent>(TEXT("ArrowComponent"));
	m_ArrowComponent->SetupAttachment(m_Box);
	m_ArrowComponent->SetArrowColor(FLinearColor(255.f, 200.f, 150.f, 255.f));

	m_Box->SetBoxExtent(FVector(1.f, 1.f, 0.1f));
}