#include "WarningMark.h"

#include <Components/DecalComponent.h>

AWarningMark::AWarningMark()
{
	m_Decal = CreateDefaultSubobject<UDecalComponent>(TEXT("DecalComponent"));
	SetRootComponent(m_Decal);

	static ConstructorHelpers::FObjectFinder<UMaterialInterface> WarningMarkMaterial(TEXT("Material'/Game/Game/Asset/Material/MT_WarningMark.MT_WarningMark'"));
	if (WarningMarkMaterial.Succeeded())
		m_Decal->SetMaterial(0, WarningMarkMaterial.Object);

	SetActorRotation(FRotator(-90.f, 0.f, 0.f));
}