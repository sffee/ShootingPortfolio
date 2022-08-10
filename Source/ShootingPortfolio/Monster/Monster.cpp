#include "Monster.h"

#include "MonsterAIController.h"
#include "ShootingPortfolio/Player/PlayerCharacter.h"
#include "ShootingPortfolio/UI/DamageText/DamageTextActor.h"

#include "ShootingPortfolio/GameInstance/ShootingGameInstance.h"

AMonster::AMonster()
	: m_IsAttacking(false)
{
	PrimaryActorTick.bCanEverTick = true;

	AIControllerClass = AMonsterAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	bUseControllerRotationYaw = false;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 540.f, 0.f);
	GetCharacterMovement()->bOrientRotationToMovement = true;

	m_RightWeaponCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("RightWeaponCollsiion"));
	m_RightWeaponCollision->SetupAttachment(GetMesh(), TEXT("RightWeaponSocket"));
	m_RightWeaponCollision->SetCollisionProfileName(TEXT("MonsterAttack"));

	m_LeftWeaponCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("LeftWeaponCollsiion"));
	m_LeftWeaponCollision->SetupAttachment(GetMesh(), TEXT("LeftWeaponSocket"));
	m_LeftWeaponCollision->SetCollisionProfileName(TEXT("MonsterAttack"));

	m_RightWeaponCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	m_LeftWeaponCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	GetCapsuleComponent()->SetCollisionProfileName(FName("Monster"));
	GetMesh()->SetCollisionProfileName(FName("Monster"));
	GetMesh()->SetCollisionResponseToChannel(COLLISION_PLAYER, ECR_Ignore);

	GetMesh()->bReceivesDecals = false;
}

void AMonster::BeginPlay()
{
	Super::BeginPlay();

	OnTakeAnyDamage.AddDynamic(this, &AMonster::ReceiveDamage);
	m_RightWeaponCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	m_Status.CurHP = m_Status.MaxHP;
	m_Status.CurShield = m_Status.MaxShield;

	m_RightWeaponCollision->OnComponentBeginOverlap.AddDynamic(this, &AMonster::OnBeginOverlap);
	m_LeftWeaponCollision->OnComponentBeginOverlap.AddDynamic(this, &AMonster::OnBeginOverlap);

	InitAttackInfoDataTable();

	UAnimInstance* AnimInst = GetMesh()->GetAnimInstance();
	if (AnimInst)
		AnimInst->OnMontageEnded.AddDynamic(this, &AMonster::OnAttackMontageEnded);
}

void AMonster::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMonster::InitAttackInfoDataTable()
{
	UShootingGameInstance* GameInstance = Cast<UShootingGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (GameInstance == nullptr)
		return;

	UDataTable* AttackInfoDataTable = GameInstance->GetMonsterAttackInfoDataTable(m_Name);
	if (AttackInfoDataTable == nullptr)
		return;

	TArray<FMonsterAttackInfo*> AllRows;
	AttackInfoDataTable->GetAllRows<FMonsterAttackInfo>(TEXT(""), AllRows);

	TArray<FName> AllRowNames = AttackInfoDataTable->GetRowNames();

	for (int32 i = 0; i < AllRows.Num(); i++)
		m_AttackCooltimes.Add(AllRowNames[i], GetWorld()->GetTimeSeconds());
}

bool AMonster::PlayAttackSection(const FName& _SectionName)
{
	UShootingGameInstance* GameInstance = Cast<UShootingGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (GameInstance == nullptr)
		return false;

	UDataTable* AttackInfoDataTable = GameInstance->GetMonsterAttackInfoDataTable(m_Name);
	if (AttackInfoDataTable == nullptr)
		return false;

	FMonsterAttackInfo* AttackInfo = AttackInfoDataTable->FindRow<FMonsterAttackInfo>(_SectionName, TEXT(""));
	if (m_AttackMontage == nullptr || AttackInfo == nullptr)
		return false;

	if (m_AttackCooltimes.Contains(_SectionName) == false)
		return false;

	float WorldTime = GetWorld()->GetTimeSeconds();
	if (WorldTime < m_AttackCooltimes[_SectionName])
		return false;

	m_AttackCooltimes[_SectionName] = WorldTime + AttackInfo->Cooltime;

	PlayMontage(m_AttackMontage, _SectionName);
	m_CurPlayAttackSectionName = _SectionName;

	m_IsAttacking = true;

	return true;
}

void AMonster::PlayMontage(UAnimMontage* _AnimMontage, FName _SectionName)
{
	if (_AnimMontage == nullptr)
		return;

	UAnimInstance* AnimInst = GetMesh()->GetAnimInstance();
	if (AnimInst)
	{
		AnimInst->Montage_Play(_AnimMontage);
		AnimInst->Montage_JumpToSection(_SectionName);
	}
}

void AMonster::SpawnDamageText(const APlayerCharacter* _Player, float _Damage)
{
	float RandX = FMath::FRandRange(-30.f, 30.f);
	float RandY = FMath::FRandRange(-30.f, 30.f);

	FVector SpawnLocation = _Player->GetActorLocation() + FVector(0.f, 0.f, _Player->GetCapsuleComponent()->GetScaledCapsuleHalfHeight()) + FVector(RandX, RandY, 0.f);

	ADamageTextActor* DamageTextActor = GetWorld()->SpawnActor<ADamageTextActor>(ADamageTextActor::StaticClass(), SpawnLocation, FRotator::ZeroRotator);
	if (DamageTextActor)
		DamageTextActor->SetData((int32)_Damage, FLinearColor::Red);
}

void AMonster::ReceiveDamage(AActor* _DamagedActor, float _Damage, const UDamageType* _DamageType, class AController* _InstigatorController, AActor* _DamageCauser)
{
	float Damage = _Damage;

	if (0.f < m_Status.CurShield)
	{
		if (m_Status.CurShield <= Damage)
		{
			Damage -= m_Status.CurShield;
			m_Status.CurShield = 0.f;
		}
		else
		{
			m_Status.CurShield -= Damage;
			Damage = 0.f;
		}
	}

	m_Status.CurHP -= Damage;
}

void AMonster::OnBeginOverlap(UPrimitiveComponent* _PrimitiveComponent, AActor* _OtherActor, UPrimitiveComponent* _OtherComp, int32 _OtherBodyIndex, bool _bFromSweep, const FHitResult& _SweepResult)
{
	APlayerCharacter* Player = Cast<APlayerCharacter>(_OtherActor);
	if (Player == nullptr || Controller == nullptr)
		return;

	UShootingGameInstance* GameInstance = Cast<UShootingGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (GameInstance == nullptr)
		return;

	UDataTable* AttackInfoDataTable = GameInstance->GetMonsterAttackInfoDataTable(m_Name);
	if (AttackInfoDataTable == nullptr)
		return;

	FMonsterAttackInfo* AttackInfo = AttackInfoDataTable->FindRow<FMonsterAttackInfo>(m_CurPlayAttackSectionName, TEXT(""));
	if (AttackInfo == nullptr)
		return;

	SpawnDamageText(Player, AttackInfo->Damage);
	UGameplayStatics::ApplyDamage(Player, AttackInfo->Damage, Controller, this, UDamageType::StaticClass());
}

void AMonster::OnAttackMontageEnded(UAnimMontage* _Montage, bool _bInterrupted)
{
	if (_Montage != m_AttackMontage)
		return;

	m_IsAttacking = false;
}

void AMonster::RightWeaponCollisionEnable()
{
	if (m_RightWeaponCollision == nullptr)
		return;

	m_RightWeaponCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void AMonster::RightWeaponCollisionDisable()
{
	if (m_RightWeaponCollision == nullptr)
		return;

	m_RightWeaponCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AMonster::LeftWeaponCollisionEnable()
{
	if (m_LeftWeaponCollision == nullptr)
		return;

	m_LeftWeaponCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void AMonster::LeftWeaponCollisionDisable()
{
	if (m_LeftWeaponCollision == nullptr)
		return;

	m_LeftWeaponCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}