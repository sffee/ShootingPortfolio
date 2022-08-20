#include "PlayerCharacterController.h"

#include "PlayerCharacter.h"
#include "ShootingPortfolio/Weapon/Weapon.h"
#include "ShootingPortfolio/UI/ShootingHUD.h"
#include "ShootingPortfolio/UI/PlayerOverlayWidget.h"
#include "ShootingPortfolio/UI/WeaponSlotWidget.h"
#include "ShootingPortfolio/UI/WeaponInventoryWidget.h"
#include "ShootingPortfolio/UI/WaveMonsterCountWidget.h"
#include "ShootingPortfolio/UI/MonsterCountBlockWidget.h"
#include "ShootingPortfolio/UI/SniperRifleScopeWidget.h"
#include "ShootingPortfolio/Weapon/Weapon.h"
#include "ShootingPortfolio/Monster/Monster.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/ListView.h"
#include "ShootingPortfolio/GameMode/ShootingGameMode.h"
#include "ShootingPortfolio/Monster/SpawnMonsterData.h"

APlayerCharacterController::APlayerCharacterController()
	: m_StartWaveCountTime(60.f)
	, m_RemainingWaveCountTime(0.f)
	, m_PrevReminingWaveCountTime(0.f)
	, m_FirstHUDUpdateComplete(false)
	, m_WaveState(EWaveState::MAX)
{
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FClassFinder<UUserWidget> SniperRifleScopeWidget(TEXT("WidgetBlueprint'/Game/Game/Blueprints/UI/WBP_SpierRifleScope.WBP_SpierRifleScope_C'"));
	if (SniperRifleScopeWidget.Succeeded())
		m_SniperRifleScopeWidgetClass = SniperRifleScopeWidget.Class;

	static ConstructorHelpers::FObjectFinder<USoundCue> SniperRifleZoomInSound(TEXT("SoundCue'/Game/MilitaryWeapSilver/Sound/SniperRifle/Cues/SniperRifle_ZoomIn_Cue.SniperRifle_ZoomIn_Cue'"));
	if (SniperRifleZoomInSound.Succeeded())
		m_SniperRifleZoomInSound = SniperRifleZoomInSound.Object;

	static ConstructorHelpers::FObjectFinder<USoundCue> SniperRifleZoomOutSound(TEXT("SoundCue'/Game/MilitaryWeapSilver/Sound/SniperRifle/Cues/SniperRifle_ZoomOut_Cue.SniperRifle_ZoomOut_Cue'"));
	if (SniperRifleZoomOutSound.Succeeded())
		m_SniperRifleZoomOutSound = SniperRifleZoomOutSound.Object;

	static ConstructorHelpers::FObjectFinder<USoundCue> WeaponEquipSound(TEXT("SoundCue'/Game/Game/Asset/Sound/Rifle/sw_Wep_AR_Equip_03_Cue.sw_Wep_AR_Equip_03_Cue'"));
	if (WeaponEquipSound.Succeeded())
		m_WeaponEquipSound = WeaponEquipSound.Object;
}

void APlayerCharacterController::BeginPlay()
{
	Super::BeginPlay();

	m_Player = Cast<APlayerCharacter>(GetPawn());
	m_HUD = Cast<AShootingHUD>(GetHUD());

	m_Status.CurHP = m_Status.MaxHP;
	m_Status.CurStamina = m_Status.MaxStamina;

	InitAmmo();

	if (m_SniperRifleScopeWidgetClass)
	{
		m_SniperRifleScopeWidget = CreateWidget<USniperRifleScopeWidget>(this, m_SniperRifleScopeWidgetClass);
		m_SniperRifleScopeWidget->AddToViewport();
		m_SniperRifleScopeWidget->SetVisibility(ESlateVisibility::Hidden);
	}
}

void APlayerCharacterController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdateFirstHUD();
	UpdateWaveCountdown(DeltaTime);
}

void APlayerCharacterController::SetWaveState(EWaveState _State)
{
	m_WaveState = _State;

	AShootingGameMode* GameMode = Cast<AShootingGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (GameMode)
		GameMode->SetWaveState(_State);

	switch (_State)
	{
	case EWaveState::Countdown:
		WaveCountdown();
		break;
	case EWaveState::Start:
		WaveStart();
		break;
	case EWaveState::Play:
		WavePlay();
		break;
	case EWaveState::Complete:
		WaveComplete();
		break;
	default:
		break;
	}
}

void APlayerCharacterController::WaveCountdown()
{
	m_HUD = m_HUD == nullptr ? Cast<AShootingHUD>(GetHUD()) : m_HUD;
	if (m_HUD == nullptr ||
		m_HUD->m_PlayerOverlayWidget == nullptr ||
		m_HUD->m_PlayerOverlayWidget->WaveNumberText == nullptr ||
		m_HUD->m_PlayerOverlayWidget->WaveCountdownText == nullptr ||
		m_HUD->m_PlayerOverlayWidget->WaveStartCompleteText == nullptr ||
		m_HUD->m_PlayerOverlayWidget->SkipWaveText == nullptr)
		return;

	m_RemainingWaveCountTime = m_StartWaveCountTime;

	m_HUD->m_PlayerOverlayWidget->WaveStartCompleteText->SetVisibility(ESlateVisibility::Hidden);
	m_HUD->m_PlayerOverlayWidget->WaveNumberText->SetVisibility(ESlateVisibility::Hidden);
	m_HUD->m_PlayerOverlayWidget->WaveCountdownText->SetVisibility(ESlateVisibility::Visible);
	m_HUD->m_PlayerOverlayWidget->SkipWaveText->SetVisibility(ESlateVisibility::Visible);
}

void APlayerCharacterController::WaveStart()
{
	m_HUD = m_HUD == nullptr ? Cast<AShootingHUD>(GetHUD()) : m_HUD;
	if (m_HUD == nullptr ||
		m_HUD->m_PlayerOverlayWidget == nullptr ||
		m_HUD->m_PlayerOverlayWidget->WaveCountdownText == nullptr ||
		m_HUD->m_PlayerOverlayWidget->WaveStartCompleteText == nullptr ||
		m_HUD->m_PlayerOverlayWidget->SkipWaveText == nullptr ||
		m_HUD->m_PlayerOverlayWidget->WaveStartAnimation == nullptr)
		return;

	m_HUD->m_PlayerOverlayWidget->WaveCountdownText->SetVisibility(ESlateVisibility::Hidden);
	m_HUD->m_PlayerOverlayWidget->WaveStartCompleteText->SetVisibility(ESlateVisibility::Visible);
	m_HUD->m_PlayerOverlayWidget->WaveStartCompleteText->SetText(FText::FromString(FString("Wave Start!")));
	m_HUD->m_PlayerOverlayWidget->SkipWaveText->SetVisibility(ESlateVisibility::Hidden);

	m_HUD->m_PlayerOverlayWidget->PlayAnimation(m_HUD->m_PlayerOverlayWidget->WaveStartAnimation);

	GetWorldTimerManager().SetTimer(m_WaveTimer, this, &APlayerCharacterController::WaveStartTimerEnd, m_HUD->m_PlayerOverlayWidget->WaveStartAnimation->GetEndTime());
}

void APlayerCharacterController::WaveStartTimerEnd()
{
	SetWaveState(EWaveState::Play);
}

void APlayerCharacterController::WavePlay()
{
	m_HUD = m_HUD == nullptr ? Cast<AShootingHUD>(GetHUD()) : m_HUD;
	if (m_HUD == nullptr ||
		m_HUD->m_PlayerOverlayWidget == nullptr ||
		m_HUD->m_PlayerOverlayWidget->WaveStartCompleteText == nullptr ||
		m_HUD->m_PlayerOverlayWidget->WaveNumberText == nullptr ||
		m_HUD->m_PlayerOverlayWidget->WaveMonsterCount == nullptr)
		return;

	AShootingGameMode* GameMode = Cast<AShootingGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (GameMode == nullptr)
		return;

	if (GameMode->IsBossWave())
	{
		SetHUDVisibility(false);
		GameMode->BossWave();
	}
	else
	{
		m_HUD->m_PlayerOverlayWidget->WaveStartCompleteText->SetVisibility(ESlateVisibility::Hidden);
		m_HUD->m_PlayerOverlayWidget->WaveNumberText->SetVisibility(ESlateVisibility::Visible);

		FString WaveStr = FString::Printf(TEXT("Wave %d"), GameMode->GetWaveNumber());
		m_HUD->m_PlayerOverlayWidget->WaveNumberText->SetText(FText::FromString(WaveStr));

		m_HUD->m_PlayerOverlayWidget->WaveMonsterCount->SetVisibility(ESlateVisibility::Visible);

		GameMode->SpawnStart();
	}
}

void APlayerCharacterController::WaveComplete()
{
	m_HUD = m_HUD == nullptr ? Cast<AShootingHUD>(GetHUD()) : m_HUD;
	if (m_HUD == nullptr ||
		m_HUD->m_PlayerOverlayWidget == nullptr ||
		m_HUD->m_PlayerOverlayWidget->WaveNumberText == nullptr ||
		m_HUD->m_PlayerOverlayWidget->WaveStartCompleteText == nullptr ||
		m_HUD->m_PlayerOverlayWidget->WaveCompleteAnimation == nullptr ||
		m_HUD->m_PlayerOverlayWidget->WaveMonsterCount == nullptr)
		return;

	m_HUD->m_PlayerOverlayWidget->WaveNumberText->SetVisibility(ESlateVisibility::Hidden);
	m_HUD->m_PlayerOverlayWidget->WaveStartCompleteText->SetVisibility(ESlateVisibility::Visible);
	m_HUD->m_PlayerOverlayWidget->WaveStartCompleteText->SetText(FText::FromString(FString("Wave Complete!")));
	m_HUD->m_PlayerOverlayWidget->PlayAnimation(m_HUD->m_PlayerOverlayWidget->WaveCompleteAnimation);
	m_HUD->m_PlayerOverlayWidget->WaveMonsterCount->SetVisibility(ESlateVisibility::Hidden);

	GetWorldTimerManager().SetTimer(m_WaveTimer, this, &APlayerCharacterController::WaveCompleteTimerEnd, m_HUD->m_PlayerOverlayWidget->WaveCompleteAnimation->GetEndTime());
}

void APlayerCharacterController::WaveCompleteTimerEnd()
{
	SetWaveState(EWaveState::Countdown);
}

void APlayerCharacterController::UpdateWaveCountdown(float _DeltaTime)
{
	if (m_WaveState != EWaveState::Countdown)
		return;

	if (m_RemainingWaveCountTime <= 0.f)
	{
		SetWaveState(EWaveState::Start);
		return;
	}

	m_RemainingWaveCountTime -= _DeltaTime;
	if (m_PrevReminingWaveCountTime != (int32)m_RemainingWaveCountTime)
	{
		m_HUD = m_HUD == nullptr ? Cast<AShootingHUD>(GetHUD()) : m_HUD;
		if (m_HUD == nullptr || m_HUD->m_PlayerOverlayWidget == nullptr)
			return;

		AShootingGameMode* GameMode = Cast<AShootingGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
		if (GameMode == nullptr)
			return;

		FString TimeStr = FString::Printf(TEXT("%d초 후 %d Wave가 시작됩니다."), (int32)m_RemainingWaveCountTime, GameMode->GetWaveNumber());
		m_HUD->m_PlayerOverlayWidget->WaveCountdownText->SetText(FText::FromString(TimeStr));
	}

	m_PrevReminingWaveCountTime = (int32)m_RemainingWaveCountTime;
}

void APlayerCharacterController::InitAmmo()
{
	m_AmmoMap.Add(EWeaponType::SubmachineGun, m_StartAmmo.SubmachineGun);
	m_AmmoMap.Add(EWeaponType::RocketLauncher, m_StartAmmo.RocketLauncher);
	m_AmmoMap.Add(EWeaponType::SniperRifle, m_StartAmmo.SniperRifle);
}

void APlayerCharacterController::UpdateFirstHUD()
{
	if (m_FirstHUDUpdateComplete || m_HUD->GetPlayerOverlayWidget() == nullptr)
		return;

	UpdateHPHUD();
	UpdateStaminaHUD();
	UpdateAmmoHUD();

	m_FirstHUDUpdateComplete = true;
}

void APlayerCharacterController::SniperZoomIn()
{
	if (m_SniperRifleScopeWidget == nullptr)
		return;

	m_SniperRifleScopeWidget->SetVisibility(ESlateVisibility::Visible);
	m_SniperRifleScopeWidget->PlayAnimation(m_SniperRifleScopeWidget->ZoomAnimation);

	m_HUD = m_HUD == nullptr ? Cast<AShootingHUD>(GetHUD()) : m_HUD;
	if (m_HUD)
		m_HUD->SetDrawCrosshair(false);

	if (m_SniperRifleZoomInSound)
		UGameplayStatics::PlaySound2D(this, m_SniperRifleZoomInSound);
}

void APlayerCharacterController::SniperZoomOut()
{
	if (m_SniperRifleScopeWidget == nullptr)
		return;

	m_SniperRifleScopeWidget->PlayAnimation(m_SniperRifleScopeWidget->ZoomAnimation, 0.f, 1, EUMGSequencePlayMode::Reverse);

	m_HUD = m_HUD == nullptr ? Cast<AShootingHUD>(GetHUD()) : m_HUD;
	if (m_HUD)
		m_HUD->SetDrawCrosshair(true);

	if (m_SniperRifleZoomOutSound)
		UGameplayStatics::PlaySound2D(this, m_SniperRifleZoomOutSound);
}

void APlayerCharacterController::AddHP(float _Value)
{
	m_Status.CurHP = FMath::Clamp(m_Status.CurHP + _Value, 0.f, m_Status.MaxHP);

	UpdateHPHUD();
}

void APlayerCharacterController::AddStamina(float _Value)
{
	m_Status.CurStamina = FMath::Clamp(m_Status.CurStamina + _Value, 0.f, m_Status.MaxStamina);

	UpdateStaminaHUD();
}

void APlayerCharacterController::AddAmmo(int32 _Amount)
{
	m_Player = m_Player == nullptr ? Cast<APlayerCharacter>(GetPawn()) : m_Player;
	if (m_Player == nullptr)
		return;

	AWeapon* Weapon = m_Player->GetEquipWeapon();
	if (Weapon == nullptr)
		return;

	Weapon->AddAmmo(_Amount);
	UpdateAmmoHUD();
}

void APlayerCharacterController::AddAmmoMap(EWeaponType _WeaponType, int32 _Amount)
{
	if (m_AmmoMap.Contains(_WeaponType) == false)
		return;

	m_AmmoMap[_WeaponType].CurAmmo = FMath::Clamp(m_AmmoMap[_WeaponType].CurAmmo + _Amount, 0, m_AmmoMap[_WeaponType].MaxAmmo);

	UpdateAmmoHUD();
}

void APlayerCharacterController::AddAllAmmoRate(float _Rate)
{
	for (auto& Ammo : m_AmmoMap)
	{
		int32 AddAmmo = Ammo.Value.MaxAmmo * _Rate;
		Ammo.Value.CurAmmo = FMath::Clamp(Ammo.Value.CurAmmo + AddAmmo, 0, Ammo.Value.MaxAmmo);
	}

	UpdateAmmoHUD();
}

void APlayerCharacterController::SubAmmo()
{
	m_Player = m_Player == nullptr ? Cast<APlayerCharacter>(GetPawn()) : m_Player;
	if (m_Player == nullptr)
		return;

	AWeapon* Weapon = m_Player->GetEquipWeapon();
	if (Weapon == nullptr)
		return;

	Weapon->AddAmmo(-1);
	UpdateAmmoHUD();
}

void APlayerCharacterController::ReloadFinish()
{
	m_Player = m_Player == nullptr ? Cast<APlayerCharacter>(GetPawn()) : m_Player;
	if (m_Player == nullptr)
		return;

	AWeapon* Weapon = m_Player->GetEquipWeapon();
	if (Weapon == nullptr)
		return;

	if (Weapon->GetInfinityMagazine())
	{
		Weapon->SetAmmo(Weapon->GetMagazine());
	}
	else
	{
		int32 WeaponAmmo = Weapon->GetAmmo();
		int32 WeaponMagazine = Weapon->GetMagazine();
		FAmmoData& AmmoMap = m_AmmoMap[Weapon->GetWeaponType()];

		int32 Ammo = FMath::Clamp(WeaponMagazine - WeaponAmmo, 0, WeaponMagazine);
		int32 ReloadAmmo = AmmoMap.CurAmmo < Ammo ? AmmoMap.CurAmmo : Ammo;

		Weapon->AddAmmo(ReloadAmmo);
		AmmoMap.CurAmmo -= ReloadAmmo;
	}

	UpdateAmmoHUD();
}

bool APlayerCharacterController::AmmoMapEmpty(EWeaponType _Type)
{
	if (m_AmmoMap.Contains(_Type) == false)
		return true;

	if (m_AmmoMap[_Type].CurAmmo == 0)
		return true;

	return false;
}

void APlayerCharacterController::ChangeWeapon(AWeapon* _Weapon, int32 _SlotIndex)
{
	if (_SlotIndex < 0 || m_WeaponInventory.Num() <= _SlotIndex)
		return;

	if (_Weapon->GetInventorySlotIndex() == _SlotIndex)
		return;

	m_Player = m_Player == nullptr ? Cast<APlayerCharacter>(GetPawn()) : m_Player;
	if (m_Player == nullptr)
		return;

	PlayChangeWeaponAnimation(_Weapon, m_WeaponInventory[_SlotIndex]);
	_Weapon->GetMesh()->SetVisibility(false);
	m_WeaponInventory[_SlotIndex]->GetMesh()->SetVisibility(true);

	m_Player->PlayMontage(m_Player->GetEquipWeaponMontage(), TEXT("Equip"));
	m_Player->EquipWeapon(m_WeaponInventory[_SlotIndex]);
	m_Player->SetState(EPlayerState::Equipping);

	UpdateAmmoHUD();

	if (m_WeaponEquipSound)
		UGameplayStatics::PlaySound2D(GetWorld(), m_WeaponEquipSound);
}

void APlayerCharacterController::PlayChangeWeaponAnimation(const AWeapon* _OldWeapon, const AWeapon* _NewWeapon)
{
	m_HUD = m_HUD == nullptr ? Cast<AShootingHUD>(GetHUD()) : m_HUD;
	if (m_HUD == nullptr || m_HUD->m_PlayerOverlayWidget == nullptr)
		return;

	if (_OldWeapon)
		m_HUD->m_PlayerOverlayWidget->WeaponInventory->PlayAnimation(GetWeaponSlotAnimation(_OldWeapon), 0.f, 1, EUMGSequencePlayMode::Reverse);

	if (_NewWeapon)
		m_HUD->m_PlayerOverlayWidget->WeaponInventory->PlayAnimation(GetWeaponSlotAnimation(_NewWeapon));
}

bool APlayerCharacterController::AmmoMapIsFull() const
{
	bool Result = true;

	for (const auto& Ammo : m_AmmoMap)
	{
		if (Ammo.Value.CurAmmo != Ammo.Value.MaxAmmo)
		{
			Result = false;
			break;
		}
	}

	return Result;
}

void APlayerCharacterController::UpdateHPHUD()
{
	m_HUD = m_HUD == nullptr ? Cast<AShootingHUD>(GetHUD()) : m_HUD;
	if (m_HUD == nullptr)
		return;

	m_HUD->SetHP(m_Status.CurHP, m_Status.MaxHP);
}

void APlayerCharacterController::UpdateStaminaHUD()
{
	m_HUD = m_HUD == nullptr ? Cast<AShootingHUD>(GetHUD()) : m_HUD;
	if (m_HUD == nullptr)
		return;

	m_HUD->SetStamina(m_Status.CurStamina, m_Status.MaxStamina);
}

void APlayerCharacterController::UpdateAmmoHUD()
{
	m_Player = m_Player == nullptr ? Cast<APlayerCharacter>(GetPawn()) : m_Player;
	m_HUD = m_HUD == nullptr ? Cast<AShootingHUD>(GetHUD()) : m_HUD;
	if (m_HUD == nullptr || m_Player == nullptr)
		return;

	AWeapon* EquipWeapon = m_Player->GetEquipWeapon();
	if (EquipWeapon == nullptr)
		return;

	if (EquipWeapon->GetInfinityMagazine())
	{
		m_HUD->SetAmmo(EquipWeapon->GetAmmo(), -1);
	}
	else
	{
		EWeaponType WeaponType = EquipWeapon->GetWeaponType();
		if (m_AmmoMap.Contains(WeaponType) == false)
			return;

		m_HUD->SetAmmo(EquipWeapon->GetAmmo(), m_AmmoMap[WeaponType].CurAmmo);
	}

	for (const auto& Weapon : m_WeaponInventory)
	{
		if (Weapon->GetInfinityMagazine())
		{
			UWeaponSlotWidget* SlotWidget = GetWeaponSlotWidget(Weapon);
			SlotWidget->AmmoText->SetText(FText::AsNumber(Weapon->GetAmmo()));
		}
		else
		{
			EWeaponType WeaponType = Weapon->GetWeaponType();
			if (m_AmmoMap.Contains(WeaponType) == false)
				return;

			int32 Ammo = m_AmmoMap[WeaponType].CurAmmo + Weapon->GetAmmo();
			UWeaponSlotWidget* SlotWidget = GetWeaponSlotWidget(Weapon);
			SlotWidget->AmmoText->SetText(FText::AsNumber(Ammo));
		}
	}
}

void APlayerCharacterController::AddWeapon(AWeapon* _Weapon)
{
	_Weapon->SetInventorySlotIndex(m_WeaponInventory.Num());
	m_WeaponInventory.Add(_Weapon);

	UWeaponSlotWidget* WeaponSlotWidget = GetWeaponSlotWidget(_Weapon);
	if (WeaponSlotWidget == nullptr)
		return;

	WeaponSlotWidget->BackgroundImage->SetVisibility(ESlateVisibility::Visible);

	WeaponSlotWidget->WeaponIconImage->SetVisibility(ESlateVisibility::Visible);
	WeaponSlotWidget->WeaponIconImage->SetBrushFromTexture(_Weapon->GetWeaponIcon());

	WeaponSlotWidget->AmmoText->SetVisibility(ESlateVisibility::Visible);

	if (m_AmmoMap.Contains(_Weapon->GetWeaponType()))
	{
		int32 Ammo = m_AmmoMap[_Weapon->GetWeaponType()].CurAmmo + _Weapon->GetAmmo();
		WeaponSlotWidget->AmmoText->SetText(FText::AsNumber(Ammo));
	}
	else
	{
		WeaponSlotWidget->AmmoText->SetText(FText::AsNumber(_Weapon->GetAmmo()));
	}

	if (m_WeaponInventory.Num() == 1)
		PlayChangeWeaponAnimation(nullptr, _Weapon);
}

void APlayerCharacterController::SetMonsterCountList(const TMap<UObject*, int32>& _MonsterMap)
{
	m_HUD = m_HUD == nullptr ? Cast<AShootingHUD>(GetHUD()) : m_HUD;
	if (m_HUD == nullptr ||
		m_HUD->m_PlayerOverlayWidget == nullptr ||
		m_HUD->m_PlayerOverlayWidget->WaveMonsterCount == nullptr ||
		m_HUD->m_PlayerOverlayWidget->WaveMonsterCount->MonsterList == nullptr)
		return;

	m_HUD->m_PlayerOverlayWidget->WaveMonsterCount->MonsterList->ClearListItems();

	for (const auto& Map : _MonsterMap)
	{
		AMonster* Monster = Cast<AMonster>(Map.Key);
		if (Monster == nullptr || Map.Value == 0)
			continue;

		USpawnMonsterData* Data = NewObject<USpawnMonsterData>();
		Data->SetMonsterName(Monster->GetMonsterName());
		Data->SetSpawnCount(Map.Value);

		m_HUD->m_PlayerOverlayWidget->WaveMonsterCount->MonsterList->AddItem(Data);
	}
}

void APlayerCharacterController::SetHUDVisibility(bool _Set)
{
	m_HUD = m_HUD == nullptr ? Cast<AShootingHUD>(GetHUD()) : m_HUD;
	if (m_HUD == nullptr || m_HUD->m_PlayerOverlayWidget == nullptr)
		return;

	if (_Set)
		m_HUD->SetHUDVisibility(true);
	else
		m_HUD->SetHUDVisibility(false);
}

UWeaponSlotWidget* APlayerCharacterController::GetWeaponSlotWidget(const AWeapon* _Weapon)
{
	m_HUD = m_HUD == nullptr ? Cast<AShootingHUD>(GetHUD()) : m_HUD;
	if (m_HUD == nullptr || m_HUD->m_PlayerOverlayWidget == nullptr || _Weapon == nullptr)
		return nullptr;

	UWeaponSlotWidget* Widget = nullptr;

	switch (_Weapon->GetInventorySlotIndex())
	{
	case 0:
		Widget = m_HUD->m_PlayerOverlayWidget->WeaponInventory->WeaponSlot0;
		break;
	case 1:
		Widget = m_HUD->m_PlayerOverlayWidget->WeaponInventory->WeaponSlot1;
		break;
	case 2:
		Widget = m_HUD->m_PlayerOverlayWidget->WeaponInventory->WeaponSlot2;
		break;
	case 3:
		Widget = m_HUD->m_PlayerOverlayWidget->WeaponInventory->WeaponSlot3;
		break;
	default:
		break;
	}

	return Widget;
}

UWidgetAnimation* APlayerCharacterController::GetWeaponSlotAnimation(const AWeapon* _Weapon)
{
	m_HUD = m_HUD == nullptr ? Cast<AShootingHUD>(GetHUD()) : m_HUD;
	if (m_HUD == nullptr || m_HUD->m_PlayerOverlayWidget == nullptr || _Weapon == nullptr)
		return nullptr;

	UWidgetAnimation* Animation = nullptr;

	switch (_Weapon->GetInventorySlotIndex())
	{
	case 0:
		Animation = m_HUD->m_PlayerOverlayWidget->WeaponInventory->WeaponSlot0Animation;
		break;
	case 1:
		Animation = m_HUD->m_PlayerOverlayWidget->WeaponInventory->WeaponSlot1Animation;
		break;
	case 2:
		Animation = m_HUD->m_PlayerOverlayWidget->WeaponInventory->WeaponSlot2Animation;
		break;
	case 3:
		Animation = m_HUD->m_PlayerOverlayWidget->WeaponInventory->WeaponSlot3Animation;
		break;
	default:
		break;
	}

	return Animation;
}
