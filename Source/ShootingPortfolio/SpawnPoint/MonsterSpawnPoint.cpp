#include "MonsterSpawnPoint.h"

AMonsterSpawnPoint::AMonsterSpawnPoint()
	: m_Index(0)
{
	m_Box = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	SetRootComponent(m_Box);

	m_Box->SetBoxExtent(FVector(1.f, 1.f, 0.1f));
}