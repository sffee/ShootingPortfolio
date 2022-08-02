#include "PlayerCharacterController.h"

#include "PlayerCharacter.h"
#include "ShootingPortfolio/Weapon/Weapon.h"
#include "ShootingPortfolio/UI/ShootingHUD.h"
#include "ShootingPortfolio/UI/PlayerOverlayWidget.h"
#include "ShootingPortfolio/Weapon/Weapon.h"
#include "ShootingPortfolio/Monster/Monster.h"
#include "Components/TextBlock.h"
#include "ShootingPortfolio/GameMode/ShootingGameMode.h"

APlayerCharacterController::APlayerCharacterController()
	: m_StartWaveCountTime(5.f)
	, m_RemainingWaveCountTime(0.f)
	, m_PrevReminingWaveCountTime(0.f)
	, m_FirstHUDUpdateComplete(false)
	, m_WaveState(EWaveState::MAX)
{
	PrimaryActorTick.bCanEverTick = true;
}

void APlayerCharacterController::BeginPlay()
{
	m_Player = Cast<APlayerCharacter>(GetPawn());
	m_HUD = Cast<AShootingHUD>(GetHUD());

	m_Status.CurHP = m_Status.MaxHP;
	m_Status.CurStamina = m_Status.MaxStamina;

	InitAmmo();
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
		m_HUD->m_PlayerOverlayWidget->WaveStartCompleteText == nullptr)
		return;

	m_RemainingWaveCountTime = m_StartWaveCountTime;

	m_HUD->m_PlayerOverlayWidget->WaveStartCompleteText->SetVisibility(ESlateVisibility::Hidden);
	m_HUD->m_PlayerOverlayWidget->WaveNumberText->SetVisibility(ESlateVisibility::Hidden);
	m_HUD->m_PlayerOverlayWidget->WaveCountdownText->SetVisibility(ESlateVisibility::Visible);
}

void APlayerCharacterController::WaveStart()
{
	m_HUD = m_HUD == nullptr ? Cast<AShootingHUD>(GetHUD()) : m_HUD;
	if (m_HUD == nullptr ||
		m_HUD->m_PlayerOverlayWidget == nullptr ||
		m_HUD->m_PlayerOverlayWidget->WaveCountdownText == nullptr ||
		m_HUD->m_PlayerOverlayWidget->WaveStartCompleteText == nullptr ||
		m_HUD->m_PlayerOverlayWidget->WaveStartAnimation == nullptr)
		return;

	m_HUD->m_PlayerOverlayWidget->WaveCountdownText->SetVisibility(ESlateVisibility::Hidden);
	m_HUD->m_PlayerOverlayWidget->WaveStartCompleteText->SetVisibility(ESlateVisibility::Visible);
	m_HUD->m_PlayerOverlayWidget->WaveStartCompleteText->SetText(FText::FromString(FString("Wave Start!")));

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
		m_HUD->m_PlayerOverlayWidget->WaveNumberText == nullptr)
		return;

	AShootingGameMode* GameMode = Cast<AShootingGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (GameMode == nullptr)
		return;

	m_HUD->m_PlayerOverlayWidget->WaveStartCompleteText->SetVisibility(ESlateVisibility::Hidden);
	m_HUD->m_PlayerOverlayWidget->WaveNumberText->SetVisibility(ESlateVisibility::Visible);

	FString WaveStr = FString::Printf(TEXT("Wave %d"), GameMode->GetWaveNumber());
	m_HUD->m_PlayerOverlayWidget->WaveNumberText->SetText(FText::FromString(WaveStr));

	GameMode->SpawnStart();
}

void APlayerCharacterController::WaveComplete()
{
	m_HUD = m_HUD == nullptr ? Cast<AShootingHUD>(GetHUD()) : m_HUD;
	if (m_HUD == nullptr ||
		m_HUD->m_PlayerOverlayWidget == nullptr ||
		m_HUD->m_PlayerOverlayWidget->WaveNumberText == nullptr ||
		m_HUD->m_PlayerOverlayWidget->WaveStartCompleteText == nullptr ||
		m_HUD->m_PlayerOverlayWidget->WaveCompleteAnimation == nullptr)
		return;

	m_HUD->m_PlayerOverlayWidget->WaveNumberText->SetVisibility(ESlateVisibility::Hidden);
	m_HUD->m_PlayerOverlayWidget->WaveStartCompleteText->SetVisibility(ESlateVisibility::Visible);
	m_HUD->m_PlayerOverlayWidget->WaveStartCompleteText->SetText(FText::FromString(FString("Wave Complete!")));
	m_HUD->m_PlayerOverlayWidget->PlayAnimation(m_HUD->m_PlayerOverlayWidget->WaveCompleteAnimation);

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

	int32 WeaponAmmo = Weapon->GetAmmo();
	int32 WeaponMagazine = Weapon->GetMagazine();
	int32& AmmoMap = m_AmmoMap[Weapon->GetWeaponType()];

	int32 Ammo = FMath::Clamp(WeaponMagazine - WeaponAmmo, 0, WeaponMagazine);
	int32 ReloadAmmo = AmmoMap < Ammo ? AmmoMap : Ammo;

	Weapon->AddAmmo(ReloadAmmo);
	AmmoMap -= ReloadAmmo;

	UpdateAmmoHUD();
}

bool APlayerCharacterController::AmmoMapEmpty(EWeaponType _Type)
{
	if (m_AmmoMap.Contains(_Type) == false)
		return true;

	if (m_AmmoMap[_Type] == 0)
		return true;

	return false;
}

void APlayerCharacterController::UpdateHPHUD()
{
	if (m_HUD == nullptr)
		return;

	m_HUD->SetHP(m_Status.CurHP, m_Status.MaxHP);
}

void APlayerCharacterController::UpdateStaminaHUD()
{
	if (m_HUD == nullptr)
		return;

	m_HUD->SetStamina(m_Status.CurStamina, m_Status.MaxStamina);
}

void APlayerCharacterController::UpdateAmmoHUD()
{
	m_Player = m_Player == nullptr ? Cast<APlayerCharacter>(GetPawn()) : m_Player;
	if (m_HUD == nullptr || m_Player == nullptr)
		return;

	AWeapon* Weapon = m_Player->GetEquipWeapon();
	if (Weapon == nullptr)
		return;

	EWeaponType WeaponType = Weapon->GetWeaponType();
	if (m_AmmoMap.Contains(WeaponType) == false)
		return;

	m_HUD->SetAmmo(Weapon->GetAmmo(), m_AmmoMap[WeaponType]);
}