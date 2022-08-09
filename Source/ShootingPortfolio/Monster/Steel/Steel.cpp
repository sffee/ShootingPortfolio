#include "Steel.h"

#include <AIController.h>
#include "ShootingPortfolio/Player/PlayerCharacter.h"
#include "SteelShield.h"

ASteel::ASteel()
{
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SteelMesh(TEXT("SkeletalMesh'/Game/ParagonSteel/Characters/Heroes/Steel/Meshes/Steel.Steel'"));
	if (SteelMesh.Succeeded())
		GetMesh()->SetSkeletalMesh(SteelMesh.Object);

	static ConstructorHelpers::FClassFinder<UAnimInstance> AnimInst(TEXT("AnimBlueprint'/Game/Game/Blueprints/Monster/Steel/ABP_Steel.ABP_Steel_C'"));
	if (AnimInst.Succeeded())
		GetMesh()->SetAnimInstanceClass(AnimInst.Class);

	static ConstructorHelpers::FObjectFinder<UParticleSystem> DeathParticle(TEXT("ParticleSystem'/Game/ParagonMinions/FX/Particles/Minions/Minion_melee/FX/Death/P_RangedMinion_Chunks3.P_RangedMinion_Chunks3'"));
	if (DeathParticle.Succeeded())
		m_DeathParticle = DeathParticle.Object;

	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BehaviorTree(TEXT("BehaviorTree'/Game/Game/Blueprints/Monster/Steel/BT_Steel.BT_Steel'"));
	if (BehaviorTree.Succeeded())
		m_BehaviorTree = BehaviorTree.Object;

	static ConstructorHelpers::FObjectFinder<UBlackboardData> Blackboard(TEXT("BlackboardData'/Game/Game/Blueprints/Monster/Steel/BB_Steel.BB_Steel'"));
	if (Blackboard.Succeeded())
		m_Blackboard = Blackboard.Object;

	static ConstructorHelpers::FObjectFinder<UAnimMontage> AttackMontage(TEXT("AnimMontage'/Game/Game/Blueprints/Monster/Steel/Animation/AttackMontage.AttackMontage'"));
	if (AttackMontage.Succeeded())
		m_AttackMontage = AttackMontage.Object;

	static ConstructorHelpers::FObjectFinder<UAnimMontage> ShieldBreakMontage(TEXT("AnimMontage'/Game/Game/Blueprints/Monster/Steel/Animation/ShieldBreakMontage.ShieldBreakMontage'"));
	if (ShieldBreakMontage.Succeeded())
		m_ShieldBreakMontage = ShieldBreakMontage.Object;

	static ConstructorHelpers::FObjectFinder<UParticleSystem> HitParticle(TEXT("ParticleSystem'/Game/ParagonMinions/FX/Particles/Minions/Shared/P_Minion_Melee_Impact.P_Minion_Melee_Impact'"));
	if (HitParticle.Succeeded())
		m_HitParticle = HitParticle.Object;

	static ConstructorHelpers::FClassFinder<ASteelShield> Shield(TEXT("Blueprint'/Game/Game/Blueprints/Monster/Steel/BP_SteelShield.BP_SteelShield_C'"));
	if (Shield.Succeeded())
		m_ShieldClass = Shield.Class;

	static ConstructorHelpers::FObjectFinder<UMaterialInterface> TransMaterial(TEXT("Material'/Game/Game/Asset/Material/MT_Transparent.MT_Transparent'"));
	if (TransMaterial.Succeeded())
		GetMesh()->SetMaterial(8, TransMaterial.Object);

	GetMesh()->SetWorldLocation(FVector(0.f, 0.f, -74.f));
	GetMesh()->SetWorldRotation(FRotator(0.f, -90.f, 0.f));

	GetCapsuleComponent()->SetCapsuleHalfHeight(95.f);
	GetCapsuleComponent()->SetCapsuleRadius(42.f);

	GetCharacterMovement()->MaxWalkSpeed = 300.f;

	m_LeftWeaponCollision->SetupAttachment(GetMesh(), FName("ShieldSocket"));

	if (m_HPBarWidgetComponent)
		m_HPBarWidgetComponent->SetRelativeLocation(FVector(0.f, 0.f, 180.f));

	m_Status.MaxHP = 30;

	m_Name = TEXT("Steel");
}

void ASteel::BeginPlay()
{
	Super::BeginPlay();

	SpawnAndEquipShield();

	AAIController* AIController = Cast<AAIController>(Controller);
	if (AIController)
	{
		UBlackboardComponent* BlackboardComponent = AIController->GetBlackboardComponent();
		if (BlackboardComponent)
			BlackboardComponent->SetValueAsBool(TEXT("EquipShield"), true);
	}
}

void ASteel::Destroyed()
{
	if (m_Shield)
		m_Shield->Destroy();

	if (m_DeathParticle)
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), m_DeathParticle, GetActorTransform());

	Super::Destroyed();
}

void ASteel::RightWeaponCollisionEnable()
{
	if (m_Shield && m_SteelState == ESteelState::Shield)
		m_Shield->RightWeaponCollisionEnable();
	else if (m_SteelState == ESteelState::Idle)
		Super::RightWeaponCollisionEnable();
}

void ASteel::RightWeaponCollisionDisable()
{
	if (m_Shield && m_SteelState == ESteelState::Shield)
		m_Shield->RightWeaponCollisionDisable();
	else if (m_SteelState == ESteelState::Idle)
		Super::RightWeaponCollisionDisable();
}

void ASteel::SpawnAndEquipShield()
{
	if (m_ShieldClass == nullptr)
		return;

	m_Shield = GetWorld()->SpawnActor<ASteelShield>(m_ShieldClass);

	if (m_Shield)
	{
		m_Shield->SetOwner(this);

		const USkeletalMeshSocket* Socket = GetMesh()->GetSocketByName(TEXT("ShieldSocket"));
		if (Socket)
			Socket->AttachActor(m_Shield, GetMesh());
	}
}

void ASteel::SetSteelState(ESteelState _State)
{
	m_SteelState = _State;

	switch (_State)
	{
	case ESteelState::Shield:
		break;
	case ESteelState::ShieldBreak:
		Controller->StopMovement();
		PlayMontage(m_ShieldBreakMontage, TEXT("Break"));
		break;
	case ESteelState::Idle:
		GetCharacterMovement()->MaxWalkSpeed = 500.f;
		break;
	default:
		break;
	}
}