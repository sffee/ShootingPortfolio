#include "Khaimera.h"

AKhaimera::AKhaimera()
{
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> KhaimeraMesh(TEXT("SkeletalMesh'/Game/ParagonKhaimera/Characters/Heroes/Khaimera/Skins/Tier2/GruxPelt/Meshes/Khaimera_GruxPelt.Khaimera_GruxPelt'"));
	if (KhaimeraMesh.Succeeded())
		GetMesh()->SetSkeletalMesh(KhaimeraMesh.Object);

	static ConstructorHelpers::FClassFinder<UAnimInstance> AnimInst(TEXT("AnimBlueprint'/Game/Game/Blueprints/Monster/Khaimera/ABP_Khaimera.ABP_Khaimera_C'"));
	if (AnimInst.Succeeded())
		GetMesh()->SetAnimInstanceClass(AnimInst.Class);

	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BehaviorTree(TEXT("BehaviorTree'/Game/Game/Blueprints/Monster/Khaimera/BT_Khaimera.BT_Khaimera'"));
	if (BehaviorTree.Succeeded())
		m_BehaviorTree = BehaviorTree.Object;

	static ConstructorHelpers::FObjectFinder<UBlackboardData> Blackboard(TEXT("BlackboardData'/Game/Game/Blueprints/Monster/Khaimera/BB_Khaimera.BB_Khaimera'"));
	if (Blackboard.Succeeded())
		m_Blackboard = Blackboard.Object;

	static ConstructorHelpers::FObjectFinder<UAnimMontage> AttackMontage(TEXT("AnimMontage'/Game/Game/Blueprints/Monster/Khaimera/Animation/AttackMontage.AttackMontage'"));
	if (AttackMontage.Succeeded())
		m_AttackMontage = AttackMontage.Object;

	static ConstructorHelpers::FObjectFinder<UParticleSystem> HitParticle(TEXT("ParticleSystem'/Game/ParagonMinions/FX/Particles/Minions/Shared/P_Minion_Melee_Impact.P_Minion_Melee_Impact'"));
	if (HitParticle.Succeeded())
		m_HitParticle = HitParticle.Object;

	static ConstructorHelpers::FObjectFinder<UDataTable> AttackInfoDataTable(TEXT("DataTable'/Game/Game/Blueprints/Monster/Khaimera/DataTable/KhaimeraAttackInfoDataTable.KhaimeraAttackInfoDataTable'"));
	if (AttackInfoDataTable.Succeeded())
		m_AttackInfoDataTable = AttackInfoDataTable.Object;

	GetMesh()->SetWorldLocation(FVector(0.f, 0.f, -74.f));
	GetMesh()->SetWorldRotation(FRotator(0.f, -90.f, 0.f));

	GetCapsuleComponent()->SetCapsuleHalfHeight(70.f);
	GetCapsuleComponent()->SetCapsuleRadius(38.f);

	m_Status.MaxHP = 1000;

	m_AttackSectionNameList.Add(FName("Attack1"));
}

void AKhaimera::BeginPlay()
{
	Super::BeginPlay();
}
