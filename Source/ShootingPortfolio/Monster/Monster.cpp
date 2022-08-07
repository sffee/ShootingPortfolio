#include "Monster.h"

#include "MonsterAIController.h"
#include "ShootingPortfolio/Player/PlayerCharacter.h"
#include "ShootingPortfolio/UI/DamageText/DamageTextActor.h"

AMonster::AMonster()
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

	GetCapsuleComponent()->SetCollisionProfileName(FName("Monster"));
	GetMesh()->SetCollisionProfileName(FName("Monster"));
}

void AMonster::BeginPlay()
{
	Super::BeginPlay();

	OnTakeAnyDamage.AddDynamic(this, &AMonster::ReceiveDamage);
	m_RightWeaponCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	m_Status.CurHP = m_Status.MaxHP;

	m_RightWeaponCollision->OnComponentBeginOverlap.AddDynamic(this, &AMonster::OnBeginOverlap);
}

void AMonster::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

bool AMonster::PlayAttackIndex(int32 _Index)
{
	if (m_AttackMontage == nullptr || _Index < m_AttackSectionNameList.Num() - 1)
		return false;

	PlayMontage(m_AttackMontage, m_AttackSectionNameList[_Index]);
	
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
	m_Status.CurHP -= _Damage;


}

void AMonster::OnBeginOverlap(UPrimitiveComponent* _PrimitiveComponent, AActor* _OtherActor, UPrimitiveComponent* _OtherComp, int32 _OtherBodyIndex, bool _bFromSweep, const FHitResult& _SweepResult)
{
	APlayerCharacter* Player = Cast<APlayerCharacter>(_OtherActor);
	if (Player && Controller)
	{
		SpawnDamageText(Player, m_AttackDamage);
		UGameplayStatics::ApplyDamage(Player, m_AttackDamage, Controller, this, UDamageType::StaticClass());
	}
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

void AMonster::SetAttackSectionName(int32 _Index, FName _SectionName)
{
	if (m_AttackSectionNameList.Num() <= _Index)
		return;

	m_AttackSectionNameList[_Index] = _SectionName;
}