#include "RocketLauncher.h"

#include "ShootingPortfolio/Projectile/Projectile.h"

ARocketLauncher::ARocketLauncher()
{
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> WeaponMesh(TEXT("SkeletalMesh'/Game/MilitaryWeapSilver/Weapons/Rocket_Launcher_A.Rocket_Launcher_A'"));
	if (WeaponMesh.Succeeded())
		GetMesh()->SetSkeletalMesh(WeaponMesh.Object);

	static ConstructorHelpers::FObjectFinder<UTexture2D> WeaponIcon(TEXT("Texture2D'/Game/Game/Asset/Texture/Icon/RocketLauncherIcon.RocketLauncherIcon'"));
	if (WeaponIcon.Succeeded())
		m_WeaponIcon = WeaponIcon.Object;

	static ConstructorHelpers::FObjectFinder<USoundCue> FireSound(TEXT("SoundCue'/Game/MilitaryWeapSilver/Sound/RocketLauncher/Cues/RocketLauncherA_Fire_Cue.RocketLauncherA_Fire_Cue'"));
	if (FireSound.Succeeded())
		m_FireSound = FireSound.Object;

	static ConstructorHelpers::FObjectFinder<UParticleSystem> HitParticle(TEXT("ParticleSystem'/Game/MilitaryWeapSilver/FX/P_RocketLauncher_Explosion_01.P_RocketLauncher_Explosion_01'"));
	if (HitParticle.Succeeded())
		m_HitParticle = HitParticle.Object;

	static ConstructorHelpers::FClassFinder<UCameraShakeBase> FireCameraShake(TEXT("Blueprint'/Game/Game/Blueprints/Weapon/SubmachineGun/CameraShake_SubmachineGun.CameraShake_SubmachineGun_C'"));
	if (FireCameraShake.Succeeded())
		m_FireCameraShake = FireCameraShake.Class;

	static ConstructorHelpers::FObjectFinder<UAnimationAsset> FireAnimation(TEXT("AnimSequence'/Game/MilitaryWeapSilver/Weapons/Animations/Fire_RocketLauncher_W.Fire_RocketLauncher_W'"));
	if (FireAnimation.Succeeded())
		m_FireAnimation = FireAnimation.Object;

	static ConstructorHelpers::FClassFinder<AProjectile> Rocket(TEXT("Blueprint'/Game/Game/Blueprints/Weapon/RocketLauncher/BP_Rocket.BP_Rocket_C'"));
	if (Rocket.Succeeded())
		m_Projectile = Rocket.Class;

	m_Name = "RocketLauncher";
	m_AttachSocketName = "AttachRocketLauncherSocket";
	m_Ammo = 3;
	m_Magazine = 3;
	m_CrosshairType = ECrosshairType::Circle;
	m_CrosshairRecoil = 0.75f;
	m_CameraZoomFOV = 50.f;
	m_ReloadSectionName = "Reload RocketLauncher";
	m_Type = EWeaponType::RocketLauncher;
	m_Damage = 50;
	m_HeadDamageRate = 1.5f;
	m_Range = 2000.f;
	m_AimingFireBlendWeight = 0.9f;
	m_FireDelay = 1.f;
	m_IsAutoFire = false;
}