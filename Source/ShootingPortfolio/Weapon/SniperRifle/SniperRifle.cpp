#include "SniperRifle.h"

ASniperRifle::ASniperRifle()
{
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> WeaponMesh(TEXT("SkeletalMesh'/Game/MilitaryWeapSilver/Weapons/Sniper_Rifle_A.Sniper_Rifle_A'"));
	if (WeaponMesh.Succeeded())
		GetMesh()->SetSkeletalMesh(WeaponMesh.Object);

	static ConstructorHelpers::FObjectFinder<UTexture2D> WeaponIcon(TEXT("Texture2D'/Game/Game/Asset/Texture/Icon/SniperRifleIcon.SniperRifleIcon'"));
	if (WeaponIcon.Succeeded())
		m_WeaponIcon = WeaponIcon.Object;

	static ConstructorHelpers::FObjectFinder<USoundCue> FireSound(TEXT("SoundCue'/Game/MilitaryWeapSilver/Sound/SniperRifle/Cues/SniperRifleA_Fire_Cue.SniperRifleA_Fire_Cue'"));
	if (FireSound.Succeeded())
		m_FireSound = FireSound.Object;

	static ConstructorHelpers::FObjectFinder<UParticleSystem> HitParticle(TEXT("ParticleSystem'/Game/MilitaryWeapSilver/FX/P_Impact_Metal_Large_01.P_Impact_Metal_Large_01'"));
	if (HitParticle.Succeeded())
		m_HitParticle = HitParticle.Object;

	static ConstructorHelpers::FClassFinder<UCameraShakeBase> FireCameraShake(TEXT("Blueprint'/Game/Game/Blueprints/Weapon/SniperRifle/CameraShake_SniperRifle.CameraShake_SniperRifle_C'"));
	if (FireCameraShake.Succeeded())
		m_FireCameraShake = FireCameraShake.Class;

	static ConstructorHelpers::FObjectFinder<UAnimationAsset> FireAnimation(TEXT("AnimSequence'/Game/MilitaryWeapSilver/Weapons/Animations/Fire_RocketLauncher_W.Fire_RocketLauncher_W'"));
	if (FireAnimation.Succeeded())
		m_FireAnimation = FireAnimation.Object;

	m_Name = "Sniper Rifle";
	m_AttachSocketName = "AttachSniperRifleSocket";
	m_Ammo = 5;
	m_Magazine = 5;
	m_CrosshairType = ECrosshairType::Cross;
	m_CrosshairRecoil = 0.75f;
	m_CameraZoomFOV = 20.f;
	m_ReloadSectionName = "Reload SniperRifle";
	m_Type = EWeaponType::SniperRifle;
	m_Damage = 50;
	m_HeadDamageRate = 1.5f;
	m_Range = 4000.f;
	m_AimingFireBlendWeight = 0.9f;
	m_FireDelay = 0.5f;
	m_IsAutoFire = false;
}