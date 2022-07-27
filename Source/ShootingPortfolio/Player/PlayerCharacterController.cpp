#include "PlayerCharacterController.h"

#include "PlayerCharacter.h"
#include "ShootingPortfolio/Weapon/Weapon.h"
#include "ShootingPortfolio/UI/ShootingHUD.h"
#include "ShootingPortfolio/Weapon/Weapon.h"

APlayerCharacterController::APlayerCharacterController()
	: m_FirstHUDUpdateComplete(false)
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
