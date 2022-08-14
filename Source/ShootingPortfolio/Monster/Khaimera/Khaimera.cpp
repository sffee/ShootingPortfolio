#include "Khaimera.h"

#include <AIController.h>
#include "ShootingPortfolio/Decal/WarningMark.h"
#include "ShootingPortfolio/Monster/Khaimera/SpawnMonster/SpawnMonsterCircle.h"
#include "ShootingPortfolio/UI/BossMonsterHPBarWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

AKhaimera::AKhaimera()
	: m_SpawnMonsterPattern(false)
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

	static ConstructorHelpers::FClassFinder<AActor> WarningMark(TEXT("Blueprint'/Game/Game/Blueprints/Monster/Khaimera/BP_JumpAttackWarningMark.BP_JumpAttackWarningMark_C'"));
	if (WarningMark.Succeeded())
		m_WarningMarkClass = WarningMark.Class;

	static ConstructorHelpers::FClassFinder<AActor> SpawnMonsterCircle(TEXT("Blueprint'/Game/Game/Blueprints/Monster/Khaimera/BP_SpawnMonsterCircle.BP_SpawnMonsterCircle_C'"));
	if (SpawnMonsterCircle.Succeeded())
		m_SpawnMonsterCircleClass = SpawnMonsterCircle.Class;

	static ConstructorHelpers::FClassFinder<UUserWidget> BossMonsterHPBarWidget(TEXT("WidgetBlueprint'/Game/Game/Blueprints/UI/WBP_BossMonsterHPBar.WBP_BossMonsterHPBar_C'"));
	if (BossMonsterHPBarWidget.Succeeded())
		m_BossMonsterHPBarWidgetClass = BossMonsterHPBarWidget.Class;

	GetMesh()->SetWorldLocation(FVector(0.f, 0.f, -142.f));
	GetMesh()->SetWorldRotation(FRotator(0.f, -90.f, 0.f));
	GetMesh()->bComponentUseFixedSkelBounds = true;

	GetCapsuleComponent()->SetCapsuleHalfHeight(140.f);
	GetCapsuleComponent()->SetCapsuleRadius(50.f);

	m_Status.MaxHP = 100;
	m_Status.MaxShield = 50.f;

	m_Name = TEXT("Khaimera");
}

void AKhaimera::BeginPlay()
{
	Super::BeginPlay();

	if (m_BossMonsterHPBarWidgetClass)
	{
		m_BossMonsterHPBarWidget = CreateWidget<UBossMonsterHPBarWidget>(GetWorld(), m_BossMonsterHPBarWidgetClass);
		if (m_BossMonsterHPBarWidget)
		{
			m_BossMonsterHPBarWidget->AddToViewport();
			m_BossMonsterHPBarWidget->BossMonsterName->SetText(FText::FromName(m_Name));
		}

		UpdateHPBarWidget();
		UpdateShieldBarWidget();
	}

	AAIController* AIController = Cast<AAIController>(Controller);
	if (AIController)
	{
		UBlackboardComponent* BlackboardComponent = AIController->GetBlackboardComponent();
		if (BlackboardComponent)
			BlackboardComponent->SetValueAsBool(TEXT("SpawnMonster"), false);
	}
}

void AKhaimera::CreateWarningMark(const FVector& _Location)
{
	if (m_WarningMarkClass == nullptr)
		return;

	m_WarningMark = GetWorld()->SpawnActor<AWarningMark>(m_WarningMarkClass, _Location, FRotator::ZeroRotator);
}

void AKhaimera::DestroyWarningMark()
{
	if (m_WarningMark == nullptr)
		return;

	m_WarningMark->Destroy();
}

void AKhaimera::UpdateHPBarWidget()
{
	if (m_BossMonsterHPBarWidget == nullptr)
		return;

	float HPPercent = m_Status.CurHP / m_Status.MaxHP;
	m_BossMonsterHPBarWidget->HPBar->SetPercent(HPPercent);
}

void AKhaimera::UpdateShieldBarWidget()
{
	if (m_BossMonsterHPBarWidget == nullptr)
		return;

	float ShieldPercent = m_Status.CurShield / m_Status.MaxShield;
	m_BossMonsterHPBarWidget->ShieldBar->SetPercent(ShieldPercent);
}

float AKhaimera::TakeDamage(float _DamageAmount, FDamageEvent const& _DamageEvent, AController* _EventInstigator, AActor* _DamageCauser)
{
	bool IsUpdateShieldBar = 0.f < m_Status.CurShield ? true : false;

	float ActualDamage = Super::TakeDamage(_DamageAmount, _DamageEvent, _EventInstigator, _DamageCauser);

	UpdateHPBarWidget();
	if (IsUpdateShieldBar)
		UpdateShieldBarWidget();

	if (m_SpawnMonsterPattern == false)
	{
		float HPPercent = m_Status.CurHP / m_Status.MaxHP;
		if (HPPercent <= 0.5f)
		{
			m_SpawnMonsterPattern = true;

			AAIController* AIController = Cast<AAIController>(Controller);
			if (AIController)
			{
				UBlackboardComponent* BlackboardComponent = AIController->GetBlackboardComponent();
				if (BlackboardComponent)
					BlackboardComponent->SetValueAsBool(TEXT("SpawnMonster"), true);
			}
		}
	}

	return ActualDamage;
}