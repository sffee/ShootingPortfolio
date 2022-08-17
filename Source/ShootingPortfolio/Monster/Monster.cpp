#include "Monster.h"

#include "MonsterAIController.h"
#include "ShootingPortfolio/Player/PlayerCharacter.h"
#include "ShootingPortfolio/UI/DamageText/DamageTextActor.h"

#include "ShootingPortfolio/GameInstance/ShootingGameInstance.h"

AMonster::AMonster()
	: m_IsAttacking(false)
{
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

	m_Status.CurHP = m_Status.MaxHP;
	m_Status.CurShield = m_Status.MaxShield;

	m_RightWeaponCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	m_RightWeaponCollision->OnComponentBeginOverlap.AddDynamic(this, &AMonster::OnBeginOverlap);
	m_LeftWeaponCollision->OnComponentBeginOverlap.AddDynamic(this, &AMonster::OnBeginOverlap);

	InitAttackInfoDataTable();

	UAnimInstance* AnimInst = GetMesh()->GetAnimInstance();
	if (AnimInst)
		AnimInst->OnMontageEnded.AddDynamic(this, &AMonster::OnAttackMontageEnded);
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

void AMonster::SpawnDamageText(float _Damage, const FVector& _SpawnLocation, bool _IsHeadShot)
{
	FLinearColor TextColor = FLinearColor::White;
	if (_IsHeadShot)
	{
		FLinearColor OrangeColor = FLinearColor(1.f, 0.647, 0.f, 1.f);
		TextColor = OrangeColor;
	}

	float RandX = FMath::FRandRange(-30.f, 30.f);
	float RandY = FMath::FRandRange(-30.f, 30.f);

	FVector SpawnLocation = _SpawnLocation + FVector(RandX, RandY, 0.f);

	ADamageTextActor* DamageTextActor = GetWorld()->SpawnActor<ADamageTextActor>(ADamageTextActor::StaticClass(), SpawnLocation, FRotator::ZeroRotator);
	if (DamageTextActor)
		DamageTextActor->SetData((int32)_Damage, TextColor);
}

void AMonster::ApplyDamage(APlayerCharacter* _Player)
{
	if (_Player == nullptr || Controller == nullptr)
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

	UGameplayStatics::ApplyDamage(_Player, AttackInfo->Damage, Controller, this, UDamageType::StaticClass());
}

float AMonster::TakeDamage(float _DamageAmount, FDamageEvent const& _DamageEvent, AController* _EventInstigator, AActor* _DamageCauser)
{
	float ActualDamage = Super::TakeDamage(_DamageAmount, _DamageEvent, _EventInstigator, _DamageCauser);

	bool IsHeadShot = false;
	FVector SpawnLocation = GetActorLocation();
	if (_DamageEvent.IsOfType(FPointDamageEvent::ClassID))
	{
		const FPointDamageEvent* PointDamageEvent = static_cast<const FPointDamageEvent*>(&_DamageEvent);
		if (PointDamageEvent)
		{
			if (PointDamageEvent->HitInfo.BoneName == FName("head"))
				IsHeadShot = true;

			SpawnLocation = PointDamageEvent->HitInfo.ImpactPoint;
		}
	}

	SpawnDamageText(ActualDamage, SpawnLocation, IsHeadShot);

	if (0.f < m_Status.CurShield)
	{
		if (m_Status.CurShield <= _DamageAmount)
		{
			ActualDamage -= m_Status.CurShield;
			m_Status.CurShield = 0.f;
		}
		else
		{
			m_Status.CurShield -= ActualDamage;
			ActualDamage = 0.f;
		}
	}

	m_Status.CurHP -= ActualDamage;

	return ActualDamage;
}

void AMonster::OnBeginOverlap(UPrimitiveComponent* _PrimitiveComponent, AActor* _OtherActor, UPrimitiveComponent* _OtherComp, int32 _OtherBodyIndex, bool _bFromSweep, const FHitResult& _SweepResult)
{
	ApplyDamage(Cast<APlayerCharacter>(_OtherActor));
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