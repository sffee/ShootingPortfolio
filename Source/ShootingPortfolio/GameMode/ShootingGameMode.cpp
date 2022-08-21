#include "ShootingGameMode.h"

#include "ShootingPortfolio/Player/PlayerCharacterController.h"
#include "ShootingPortfolio/UI/ShootingHUD.h"
#include "ShootingPortfolio/SpawnPoint/MonsterSpawnPoint.h"
#include "ShootingPortfolio/Monster/Monster.h"
#include "ShootingPortfolio/Destructible/BossDoor.h"
#include "Components/TextBlock.h"
#include "LevelSequence/Public/LevelSequenceActor.h"
#include "LevelSequence/Public/LevelSequence.h"
#include "LevelSequence/Public/LevelSequencePlayer.h"
#include "Sound/SoundCue.h"
#include "ShootingPortfolio/PickUp/PickUp.h"

AShootingGameMode::AShootingGameMode()
	: m_Wave(1)
	, m_Phase2Wave(3)
	, m_BossWave(4)
	, m_SpawnPointCount(1)
	, m_SpawnCompleteMonsterCount(0)
	, m_NeedSpawnMonsterCount(0)
	, m_AliveMonsterCount(0)
	, m_SpawnComplete(false)
	, m_WaveState(EWaveState::Countdown)
{
	m_AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComponent"));

	static ConstructorHelpers::FClassFinder<APawn> PlayerPawn(TEXT("Blueprint'/Game/Game/Blueprints/Player/BP_PlayerCharacter.BP_PlayerCharacter_C'"));
	if (PlayerPawn.Succeeded())
		DefaultPawnClass = PlayerPawn.Class;

	static ConstructorHelpers::FClassFinder<APlayerController> PlayerController(TEXT("Blueprint'/Game/Game/Blueprints/Player/BP_PlayerCharacterController.BP_PlayerCharacterController_C'"));
	if (PlayerController.Succeeded())
		PlayerControllerClass = PlayerController.Class;

	static ConstructorHelpers::FObjectFinder<UDataTable> WaveDataTable(TEXT("DataTable'/Game/Game/Blueprints/SpawnPoint/DataTable/MonsterSpawnWaveDataTable.MonsterSpawnWaveDataTable'"));
	if (WaveDataTable.Succeeded())
		m_WaveDataTable = WaveDataTable.Object;

	static ConstructorHelpers::FObjectFinder<ULevelSequence> BossAppearLevelSequence(TEXT("LevelSequence'/Game/Game/Blueprints/Sequencer/BossSequence.BossSequence'"));
	if (BossAppearLevelSequence.Succeeded())
		m_BossAppearLevelSequence = BossAppearLevelSequence.Object;

	static ConstructorHelpers::FObjectFinder<USoundCue> BGM(TEXT("SoundCue'/Game/Game/Asset/Sound/BGM/PlayLevelBGM_Cue.PlayLevelBGM_Cue'"));
	if (BGM.Succeeded())
		m_AudioComponent->SetSound(BGM.Object);

	HUDClass = AShootingHUD::StaticClass();
}

void AShootingGameMode::BeginPlay()
{
	Super::BeginPlay();

	InitSpawnPointMap();
	InitPickUpItems();

	if (m_AudioComponent)
		m_AudioComponent->Play();

	APlayerController* Controller = GetWorld()->GetFirstPlayerController();
	if (Controller)
	{
		FInputModeGameOnly Mode;

		Controller->SetInputMode(Mode);
		Controller->bShowMouseCursor = false;
	}

	APlayerCameraManager* PlayerCameraManager = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0);
	if (PlayerCameraManager)
		PlayerCameraManager->StartCameraFade(1.f, 0.f, 3.f, FLinearColor::Black, false, true);

	GetWorldTimerManager().SetTimer(m_GameStartTimer, this, &AShootingGameMode::GameStartTimerEnd, 1.f);
}

void AShootingGameMode::GameStartTimerEnd()
{
	StartWaveCountdown();
}

void AShootingGameMode::StartWaveCountdown()
{
	m_PlayerController = m_PlayerController == nullptr ? Cast<APlayerCharacterController>(UGameplayStatics::GetPlayerController(GetWorld(), 0)) : m_PlayerController;
	if (m_PlayerController == nullptr)
		return;

	m_PlayerController->SetWaveState(EWaveState::Countdown);
}

void AShootingGameMode::StartWaveComplete()
{
	m_PlayerController = m_PlayerController == nullptr ? Cast<APlayerCharacterController>(UGameplayStatics::GetPlayerController(GetWorld(), 0)) : m_PlayerController;
	if (m_PlayerController == nullptr)
		return;

	m_PlayerController->SetWaveState(EWaveState::Complete);

	SpawnPickUpItems();
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
			Data.Scale = MonsterSpawnPoint->GetBoxComponent()->GetUnscaledBoxExtent();

			m_SpawnPointMap.Add(MonsterSpawnPoint->GetIndex(), Data);
		}
	}
}

void AShootingGameMode::InitPickUpItems()
{
	TArray<AActor*> PickUpItems;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APickUp::StaticClass(), PickUpItems);

	for (const auto& Item : PickUpItems)
	{
		APickUp* PickUpItem = Cast<APickUp>(Item);
		if (PickUpItem)
			m_PickUpItems.Add(PickUpItem);
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

	m_SpawnComplete = false;
}

void AShootingGameMode::SpawnPickUpItems()
{
	for (const auto& Item : m_PickUpItems)
		Item->SpawnItem();
}

void AShootingGameMode::SpawnStart()
{
	ResetData();

	FString WaveString = FString::Printf(TEXT("Wave%d"), m_Wave);

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

			FTimerDelegate SpawnDelegate = FTimerDelegate::CreateUObject(this, &AShootingGameMode::SpawnMonsterProcess, m_SpawnDatas[i].SpawnMonster, i);
			GetWorldTimerManager().SetTimer(m_SpawnTimers[i], SpawnDelegate, m_SpawnDatas[i].SpawnDelay, true);
		}
	}
}

void AShootingGameMode::BossWave()
{
	if (m_BossAppearLevelSequence)
	{
		m_SequencePlayer = ULevelSequencePlayer::CreateLevelSequencePlayer(GetWorld(), m_BossAppearLevelSequence, FMovieSceneSequencePlaybackSettings(), m_SequenceActor);
		m_SequencePlayer->OnFinished.AddDynamic(this, &AShootingGameMode::BossSequenceEnd);

		m_SequencePlayer->Play();
	}
}

void AShootingGameMode::BossSequenceEnd()
{
	AActor* BossDoor = UGameplayStatics::GetActorOfClass(GetWorld(), ABossDoor::StaticClass());
	if (BossDoor)
		BossDoor->Destroy();

	m_PlayerController = m_PlayerController == nullptr ? Cast<APlayerCharacterController>(UGameplayStatics::GetPlayerController(GetWorld(), 0)) : m_PlayerController;
	if (m_PlayerController)
		m_PlayerController->SetHUDVisibility(true);

	SpawnStart();
}

void AShootingGameMode::SpawnMonsterProcess(TSubclassOf<AMonster> _Monster, int32 _Index)
{
	if (_Monster == nullptr || m_SpawnDatas[_Index].SpawnCount <= m_SpawnCountMap[_Monster->GetDefaultObject()])
	{
		GetWorldTimerManager().ClearTimer(m_SpawnTimers[_Index]);
		return;
	}
	
	m_PlayerController = m_PlayerController == nullptr ? Cast<APlayerCharacterController>(UGameplayStatics::GetPlayerController(GetWorld(), 0)) : m_PlayerController;
	
	if (IsBossWave())
	{
		SpawnMonster(_Monster, m_SpawnPointMap[2]);

		m_SpawnCountMap[_Monster->GetDefaultObject()]++;
		m_AliveMonsterMap[_Monster->GetDefaultObject()]++;
		m_AliveMonsterCount++;
		m_SpawnCompleteMonsterCount++;
	}
	else
	{
		for (int32 i = 0; i < m_SpawnPointCount; i++)
		{
			SpawnMonster(_Monster, m_SpawnPointMap[i]);

			m_SpawnCountMap[_Monster->GetDefaultObject()]++;
			m_AliveMonsterMap[_Monster->GetDefaultObject()]++;
			m_AliveMonsterCount++;
			m_SpawnCompleteMonsterCount++;

			if (m_PlayerController)
				m_PlayerController->SetMonsterCountList(m_AliveMonsterMap);

			if (m_SpawnCompleteMonsterCount == m_NeedSpawnMonsterCount)
				m_SpawnComplete = true;

			if (m_SpawnDatas[i].SpawnCount <= m_SpawnCountMap[_Monster->GetDefaultObject()])
				break;
		}
	}
}

void AShootingGameMode::SpawnMonster(TSubclassOf<AMonster> _Monster, const FMonsterSpawnPointData& _SpawnPointData)
{
	FActorSpawnParameters Param;
	Param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	Param.bDeferConstruction = true;

	AMonster* Monster = GetWorld()->SpawnActor<AMonster>(_Monster, FVector::ZeroVector, FRotator::ZeroRotator, Param);
	Monster->m_MonsterDieDelegate.BindUObject(this, &AShootingGameMode::Delegate_MonsterDie);

	FVector RandomLocation = UKismetMathLibrary::RandomPointInBoundingBox(FVector::ZeroVector, _SpawnPointData.Scale);
	RandomLocation = RandomLocation.RotateAngleAxis(FMath::Abs(_SpawnPointData.Rotation.GetComponentForAxis(EAxis::Z)), FVector(0.f, 0.f, 1.f));
	RandomLocation = FVector(RandomLocation.X + _SpawnPointData.Location.X, RandomLocation.Y + _SpawnPointData.Location.Y, _SpawnPointData.Location.Z);
	
	FVector End = RandomLocation;
	End.Z -= 1000.f;

	FHitResult HitResult;
	GetWorld()->LineTraceSingleByProfile(HitResult, RandomLocation, End, FName("LandScape"));
	if (HitResult.bBlockingHit)
		RandomLocation.Z = HitResult.ImpactPoint.Z + Monster->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();

	FTransform MonsterTransform = Monster->GetActorTransform();
	MonsterTransform.SetLocation(RandomLocation);
	MonsterTransform.SetRotation(FQuat(_SpawnPointData.Rotation));
	
	Monster->FinishSpawning(MonsterTransform);
}

void AShootingGameMode::Delegate_MonsterDie(UObject* _Monster)
{	
	if (m_AliveMonsterMap.Contains(_Monster) == false || m_AliveMonsterMap[_Monster] <= 0)
		return;

	m_AliveMonsterMap[_Monster]--;
	m_AliveMonsterCount--;

	m_PlayerController = m_PlayerController == nullptr ? Cast<APlayerCharacterController>(UGameplayStatics::GetPlayerController(GetWorld(), 0)) : m_PlayerController;
	if (m_PlayerController)
		m_PlayerController->SetMonsterCountList(m_AliveMonsterMap);

	if (m_SpawnComplete && m_AliveMonsterCount <= 0)
	{
		if (IsBossWave())
		{
			int a = 0;
		}

		m_Wave++;
		StartWaveComplete();

		if (m_Wave == m_Phase2Wave)
			m_SpawnPointCount++;
	}
}
