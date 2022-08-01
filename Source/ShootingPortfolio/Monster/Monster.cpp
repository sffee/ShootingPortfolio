#include "Monster.h"

#include "MonsterAIController.h"
#include "ShootingPortfolio/Player/PlayerCharacter.h"

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

	static ConstructorHelpers::FObjectFinder<UBlackboardData> Blackboard(TEXT("BlackboardData'/Game/Game/Blueprints/Monster/BB_Monster.BB_Monster'"));
	if (Blackboard.Succeeded())
		m_Blackboard = Blackboard.Object;

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

void AMonster::ReceiveDamage(AActor* _DamagedActor, float _Damage, const UDamageType* _DamageType, class AController* _InstigatorController, AActor* _DamageCauser)
{
	m_Status.CurHP -= _Damage;


}

void AMonster::OnBeginOverlap(UPrimitiveComponent* _PrimitiveComponent, AActor* _OtherActor, UPrimitiveComponent* _OtherComp, int32 _OtherBodyIndex, bool _bFromSweep, const FHitResult& _SweepResult)
{
	APlayerCharacter* Player = Cast<APlayerCharacter>(_OtherActor);
	if (Player == nullptr || m_HitParticle == nullptr)
		return;

}