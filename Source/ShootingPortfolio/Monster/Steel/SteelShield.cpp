#include "SteelShield.h"

#include "Steel.h"
#include "SteelAnimInstance.h"
#include "ShootingPortfolio/UI/MonsterHPBarWidget.h"
#include "Components/ProgressBar.h"

ASteelShield::ASteelShield()
{
	m_CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	m_CollisionBox->SetupAttachment(GetMesh());

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> ShieldMesh(TEXT("SkeletalMesh'/Game/ParagonSteel/Characters/Heroes/Steel/Meshes/Steel.Steel'"));
	if (ShieldMesh.Succeeded())
		GetMesh()->SetSkeletalMesh(ShieldMesh.Object);

	static ConstructorHelpers::FClassFinder<UAnimInstance> AnimInst(TEXT("AnimBlueprint'/Game/Game/Blueprints/Monster/Steel/ABP_SteelShield.ABP_SteelShield_C'"));
	if (AnimInst.Succeeded())
		GetMesh()->SetAnimInstanceClass(AnimInst.Class);

	static ConstructorHelpers::FObjectFinder<UParticleSystem> HitParticle(TEXT("ParticleSystem'/Game/ParagonMinions/FX/Particles/Minions/Shared/P_Minion_Melee_Impact.P_Minion_Melee_Impact'"));
	if (HitParticle.Succeeded())
		m_HitParticle = HitParticle.Object;

	static ConstructorHelpers::FObjectFinder<UMaterialInterface> TransMaterial(TEXT("Material'/Game/Game/Asset/Material/MT_Transparent.MT_Transparent'"));
	if (TransMaterial.Succeeded())
	{
		for (int32 i = 0; i < 8; i++)
			GetMesh()->SetMaterial(i, TransMaterial.Object);
	}

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetCharacterMovement()->GravityScale = 0.f;

	GetMesh()->bComponentUseFixedSkelBounds = true;

	m_CollisionBox->SetRelativeLocation(FVector(0.f, -78.f, 275.f));
	m_CollisionBox->SetBoxExtent(FVector(110.f, 5.f, 80.f));
	m_CollisionBox->SetCollisionProfileName(FName("MonsterShield"));

	m_Status.MaxHP = 60;
}

void ASteelShield::BeginPlay()
{
	Super::BeginPlay();

	if (m_HPBarWidgetComponent)
	{
		m_HPBarWidget = Cast<UMonsterHPBarWidget>(m_HPBarWidgetComponent->GetWidget());
		
		if (m_HPBarWidget)
			m_HPBarWidget->HPBar->SetFillColorAndOpacity(FLinearColor::Yellow);
	}
}

void ASteelShield::Destroyed()
{
	ASteel* Steel = Cast<ASteel>(GetOwner());
	if (Steel)
		Steel->SetSteelState(ESteelState::ShieldBreak);

	Super::Destroyed();
}