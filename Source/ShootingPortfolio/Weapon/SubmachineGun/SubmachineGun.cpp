#include "SubmachineGun.h"

ASubmachineGun::ASubmachineGun()
{
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> WeaponMesh(TEXT("SkeletalMesh'/Game/Game/Asset/Mesh/Gun/SMG/SMG.SMG'"));
	if (WeaponMesh.Succeeded())
		GetMesh()->SetSkeletalMesh(WeaponMesh.Object);

	static ConstructorHelpers::FObjectFinder<USoundCue> FireSound(TEXT("SoundCue'/Game/Game/Asset/Sound/Gunshots/AR_Shot_1_Cue.AR_Shot_1_Cue'"));
	if (FireSound.Succeeded())
		m_FireSound = FireSound.Object;

	static ConstructorHelpers::FObjectFinder<UParticleSystem> MuzzleFlash(TEXT("ParticleSystem'/Game/Game/Asset/FX/P_BelicaMuzzle_single_burst.P_BelicaMuzzle_single_burst'"));
	if (MuzzleFlash.Succeeded())
		m_MuzzleFlashParticle = MuzzleFlash.Object;

	static ConstructorHelpers::FObjectFinder<UParticleSystem> HitParticle(TEXT("ParticleSystem'/Game/ParagonLtBelica/FX/Particles/Belica/Abilities/Primary/FX/P_BelicaHitWorld.P_BelicaHitWorld'"));
	if (HitParticle.Succeeded())
		m_HitParticle = HitParticle.Object;

	m_Ammo = 30;
	m_Magazine = 30;
	m_CrosshairType = "Cross";
	m_CrosshairRecoil = 0.75f;
	m_CameraZoomFOV = 50.f;
	m_ReloadSectionName = "Reload SMG";
	m_Type = EWeaponType::SubmachineGun;
	m_Name = "Submachine Gun";
	m_Damage = 10;
}