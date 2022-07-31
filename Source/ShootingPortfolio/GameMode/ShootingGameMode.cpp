#include "ShootingGameMode.h"

#include "ShootingPortfolio/Player/PlayerCharacterController.h"
#include "ShootingPortfolio/UI/ShootingHUD.h"
#include "ShootingPortfolio/SpawnPoint/MonsterSpawnPoint.h"
#include "ShootingPortfolio/Monster/Monster.h"

AShootingGameMode::AShootingGameMode()
	: m_Wave(1)
	, m_Phase2Wave(1)
	, m_BossWave(1)
	, m_SpawnPointCount(1)
	, m_SpawnCompleteMonsterCount(0)
	, m_NeedSpawnMonsterCount(0)
	, m_AliveMonsterCount(0)
	, m_SpawnComplete(false)
{
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FClassFinder<APawn> PlayerPawn(TEXT("Blueprint'/Game/Game/Blueprints/Player/BP_PlayerCharacter.BP_PlayerCharacter_C'"));
	if (PlayerPawn.Succeeded())
		DefaultPawnClass = PlayerPawn.Class;

	static ConstructorHelpers::FClassFinder<APlayerController> PlayerController(TEXT("Blueprint'/Game/Game/Blueprints/Player/BP_PlayerCharacterController.BP_PlayerCharacterController_C'"));
	if (PlayerController.Succeeded())
		PlayerControllerClass = PlayerController.Class;

	static ConstructorHelpers::FObjectFinder<UDataTable> WaveDataTable(TEXT("DataTable'/Game/Game/Blueprints/SpawnPoint/DataTable/MonsterSpawnWaveDataTable.MonsterSpawnWaveDataTable'"));
	if (WaveDataTable.Succeeded())
		m_WaveDataTable = WaveDataTable.Object;

	HUDClass = AShootingHUD::StaticClass();
}

void AShootingGameMode::BeginPlay()
{
	Super::BeginPlay();

	InitSpawnPointMap();
	
	SpawnStart();
}

void AShootingGameMode::Tick(float _DeltaTime)
{
	Super::Tick(_DeltaTime);

	FString Str = FString::Printf(TEXT("Monster : %d"), m_AliveMonsterCount);
	GEngine->AddOnScreenDebugMessage(0, 999.f, FColor::Blue, *Str);

	if (m_SpawnComplete)
	{
		GEngine->AddOnScreenDebugMessage(1, 999.f, FColor::Blue, TEXT("Spawn Complete"));
	}
}

void AShootingGameMode::InitSpawnPointMap()
{
	TArray<AActor*> SpawnPoints;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AMonsterSpawnPoint::StaticClass(), SpawnPoints);

	for (const auto& SpawnPoint : SpawnPoints)
	{
		AMonsterSpawnPoint* MonsterSpawnPoint = Cast<AMonsterSpawnPoint>(SpawnPoint);
		if (MonsterSpawnPoint)
		{
			FMonsterSpawnPointData Data;
			Data.SpawnPoint = MonsterSpawnPoint;
			Data.Location = MonsterSpawnPoint->GetActorLocation();
			Data.Rotation = MonsterSpawnPoint->GetActorRotation();
			Data.Scale = MonsterSpawnPoint->GetBoxComponent()->GetScaledBoxExtent();

			m_SpawnPointMap.Add(MonsterSpawnPoint->GetIndex(), Data);
		}
	}
}

void AShootingGameMode::ResetData()
{
	m_SpawnTimers.Empty();
	m_SpawnDatas.Empty();
	m_SpawnCountMap.Empty();
	m_AliveMonsterMap.Empty();

	m_SpawnCompleteMonsterCount = 0;
	m_NeedSpawnMonsterCount = 0;
	m_AliveMonsterCount = 0;
}

void AShootingGameMode::SpawnStart()
{
	ResetData();

	FString WaveString = FString::Printf(TEXT("Wave%d"), m_Wave++);

	FMonsterWave* MonsterWaveData = m_WaveDataTable->FindRow<FMonsterWave>(FName(WaveString), TEXT(""));
	if (MonsterWaveData == nullptr || MonsterWaveData->MonsterSpawnData.Num() == 0)
	{
		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 100.f, FColor::Blue, TEXT("Wave Data Error"));

		return;
	}

	for (int32 i = 0; i < MonsterWaveData->MonsterSpawnData.Num(); i++)
	{
		UObject* MonsterObject = MonsterWaveData->MonsterSpawnData[i].SpawnMonster->GetDefaultObject();
		if (m_SpawnCountMap.Contains(MonsterObject))
			checkf(false, TEXT("Monster Data Error"));

		m_SpawnCountMap.Add(MonsterObject, 0);
		m_AliveMonsterMap.Add(MonsterObject, 0);
		m_SpawnDatas.Add(MonsterWaveData->MonsterSpawnData[i]);
		m_SpawnTimers.Add(FTimerHandle());
		
		if (m_SpawnDatas[i].SpawnMonster && m_SpawnDatas[i].SpawnCount != 0)
		{
			m_NeedSpawnMonsterCount += m_SpawnDatas[i].SpawnCount;

			FTimerDelegate SpawnDelegate = FTimerDelegate::CreateUObject(this, &AShootingGameMode::SpawnMonster, m_SpawnDatas[i].SpawnMonster, i);
			GetWorldTimerManager().SetTimer(m_SpawnTimers[i], SpawnDelegate, m_SpawnDatas[i].SpawnDelay, true);
		}
	}
}

void AShootingGameMode::SpawnMonster(TSubclassOf<AMonster> _Monster, int32 _Index)
{
	if (_Monster == nullptr || m_SpawnDatas[_Index].SpawnCount <= m_SpawnCountMap[_Monster->GetDefaultObject()])
	{
		GetWorldTimerManager().ClearTimer(m_SpawnTimers[_Index]);
		return;
	}
	
	for (int32 i = 0; i < m_SpawnPointCount; i++)
	{
		SpawnMonster(_Monster, m_SpawnPointMap[i]);

		m_SpawnCountMap[_Monster->GetDefaultObject()]++;
		m_AliveMonsterMap[_Monster->GetDefaultObject()]++;
		m_AliveMonsterCount++;
		m_SpawnCompleteMonsterCount++;
		
		if (m_SpawnCompleteMonsterCount == m_NeedSpawnMonsterCount)
			m_SpawnComplete = true;

		if (m_SpawnDatas[i].SpawnCount <= m_SpawnCountMap[_Monster->GetDefaultObject()])
			break;
	}
}

void AShootingGameMode::SpawnMonster(TSubclassOf<AMonster> _Monster, const FMonsterSpawnPointData& _SpawnPointData)
{
	FActorSpawnParameters Param;
	Param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	Param.bDeferConstruction = true;

	AMonster* Monster = GetWorld()->SpawnActor<AMonster>(_Monster, FVector::ZeroVector, FRotator::ZeroRotator, Param);
	Monster->m_MonsterDieDelegate.BindUObject(this, &AShootingGameMode::Delegate_MonsterDie);

	FVector RandomLocation = UKismetMathLibrary::RandomPointInBoundingBox(_SpawnPointData.Location, _SpawnPointData.Scale);
	FVector End = RandomLocation;
	End.Z -= 1000.f;

	FHitResult HitResult;
	GetWorld()->LineTraceSingleByProfile(HitResult, RandomLocation, End, FName("LandScape"));
	if (HitResult.bBlockingHit)
		RandomLocation.Z = HitResult.ImpactPoint.Z + Monster->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();

	FTransform MonsterTransform = Monster->GetActorTransform();
	MonsterTransform.SetLocation(RandomLocation);
	
	Monster->FinishSpawning(MonsterTransform);
}

void AShootingGameMode::Delegate_MonsterDie(UObject* _Monster)
{	
	if (m_AliveMonsterMap.Contains(_Monster) == false || m_AliveMonsterMap[_Monster] <= 0)
		return;

	m_AliveMonsterMap[_Monster]--;
	m_AliveMonsterCount--;
}
