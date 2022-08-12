#include "Pistol.h"

APistol::APistol()
{
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> WeaponMesh(TEXT("SkeletalMesh'/Game/MilitaryWeapSilver/Weapons/Pistols_A.Pistols_A'"));
	if (WeaponMesh.Succeeded())
		GetMesh()->SetSkeletalMesh(WeaponMesh.Object);

	static ConstructorHelpers::FObjectFinder<UTexture2D> WeaponIcon(TEXT("Texture2D'/Game/Game/Asset/Texture/Icon/PistolIcon.PistolIcon'"));
	if (WeaponIcon.Succeeded())
		m_WeaponIcon = WeaponIcon.Object;

	static ConstructorHelpers::FObjectFinder<USoundCue> FireSound(TEXT("SoundCue'/Game/MilitaryWeapSilver/Sound/Pistol/Cues/PistolA_Fire_Cue.PistolA_Fire_Cue'"));
	if (FireSound.Succeeded())
		m_FireSound = FireSound.Object;

	static ConstructorHelpers::FObjectFinder<UParticleSystem> MuzzleFlash(TEXT("ParticleSystem'/Game/MilitaryWeapSilver/FX/P_Pistol_MuzzleFlash_01.P_Pistol_MuzzleFlash_01'"));
	if (MuzzleFlash.Succeeded())
		m_MuzzleFlashParticle = MuzzleFlash.Object;

	static ConstructorHelpers::FObjectFinder<UParticleSystem> HitParticle(TEXT("ParticleSystem'/Game/MilitaryWeapSilver/FX/P_Impact_Metal_Small_01.P_Impact_Metal_Small_01'"));
	if (HitParticle.Succeeded())
		m_HitParticle = HitParticle.Object;

	static ConstructorHelpers::FClassFinder<UCameraShakeBase> FireCameraShake(TEXT("Blueprint'/Game/Game/Blueprints/Weapon/SubmachineGun/CameraShake_SubmachineGun.CameraShake_SubmachineGun_C'"));
	if (FireCameraShake.Succeeded())
		m_FireCameraShake = FireCameraShake.Class;

	static ConstructorHelpers::FObjectFinder<UAnimationAsset> FireAnimation(TEXT("AnimSequence'/Game/MilitaryWeapSilver/Weapons/Animations/Fire_Pistol_W.Fire_Pistol_W'"));
	if (FireAnimation.Succeeded())
		m_FireAnimation = FireAnimation.Object;

	m_Name = "Pistol";
	m_AttachSocketName = "AttachPistolSocket";
	m_Ammo = 7;
	m_Magazine = 7;
	m_InfinityMagazine = true;
	m_CrosshairType = ECrosshairType::Cross;
	m_CrosshairRecoil = 0.75f;
	m_CameraZoomFOV = 50.f;
	m_ReloadSectionName = "Reload Pistol";
	m_Type = EWeaponType::Pistol;
	m_Damage = 10;
	m_HeadDamageRate = 1.5f;
	m_Range = 1000.f;
	m_AimingFireBlendWeight = 0.9f;
	m_IsAutoFire = false;
}
