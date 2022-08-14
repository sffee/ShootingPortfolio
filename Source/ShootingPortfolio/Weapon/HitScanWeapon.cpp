#include "HitScanWeapon.h"

#include "ShootingPortfolio/Player/PlayerCharacter.h"
#include "ShootingPortfolio/Monster/Monster.h"

AHitScanWeapon::AHitScanWeapon()
{
	const ConstructorHelpers::FObjectFinder<UParticleSystem> SmokeBeam(TEXT("ParticleSystem'/Game/Game/Asset/FX/SmokeBeam/P_SmokeTrail.P_SmokeTrail'"));
	if (SmokeBeam.Succeeded())
		m_SmokeBeamParticle = SmokeBeam.Object;
}

void AHitScanWeapon::Fire(const FHitResult& _TraceHitResult)
{
	CalcHitResult();

	PlaySmokeBeamParticle();
	if (m_HitResult.bBlockingHit)
		PlayHitParticle(m_HitResult.ImpactPoint);

	ApplyDamage();

	Super::Fire(_TraceHitResult);
}

void AHitScanWeapon::CalcHitResult()
{
	APlayerCharacter* Player = Cast<APlayerCharacter>(GetOwner());
	if (Player == nullptr)
		return;

	FVector2D ViewportSize;
	if (GEngine && GEngine->GameViewport)
		GEngine->GameViewport->GetViewportSize(ViewportSize);

	FVector2D CrosshairLocation(ViewportSize.X * 0.5f, ViewportSize.Y * 0.5f);
	FVector CrosshairWorldPosition;
	FVector CrosshairWorldDirection;

	bool ScreenToWorld = UGameplayStatics::DeprojectScreenToWorld(
		UGameplayStatics::GetPlayerController(this, 0),
		CrosshairLocation,
		CrosshairWorldPosition,
		CrosshairWorldDirection
	);

	float DistanceToCharacter = (Player->GetActorLocation() - CrosshairWorldPosition).Size();

	FVector SpreadStart = CrosshairWorldPosition + CrosshairWorldDirection * DistanceToCharacter;
	SpreadStart += UKismetMathLibrary::RandomUnitVector() * Player->GetCrosshairSpread() * 2.f;
	FVector SpreadEnd = SpreadStart + (CrosshairWorldDirection * m_Range);

	FHitResult SpreadHitResult;
	GetWorld()->LineTraceSingleByChannel(
		SpreadHitResult,
		SpreadStart,
		SpreadEnd,
		COLLISION_PLAYERBULLET
	);

	FVector EndLocation = SpreadHitResult.bBlockingHit ? SpreadHitResult.ImpactPoint : SpreadEnd;

	FVector Start = CrosshairWorldPosition + CrosshairWorldDirection * DistanceToCharacter;
	FVector ShootDir = (EndLocation - Start).GetSafeNormal();
	FVector End = Start + (ShootDir * m_Range);

	GetWorld()->LineTraceSingleByChannel(
		m_HitResult,
		Start,
		End,
		COLLISION_PLAYERBULLET
	);
}

void AHitScanWeapon::PlaySmokeBeamParticle()
{
	if (m_SmokeBeamParticle == nullptr)
		return;

	const USkeletalMeshSocket* BarrelSocket = GetMesh()->GetSocketByName(FName("BarrelSocket"));
	if (BarrelSocket)
	{
		UParticleSystemComponent* SmokeBeam = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), m_SmokeBeamParticle, BarrelSocket->GetSocketTransform(GetMesh()));

		FVector TargetLocation = m_HitResult.bBlockingHit ? m_HitResult.ImpactPoint : m_HitResult.TraceEnd;
		if (SmokeBeam)
			SmokeBeam->SetVectorParameter(FName("Target"), TargetLocation);
	}
}

void AHitScanWeapon::ApplyDamage()
{
	if (m_HitResult.bBlockingHit)
	{
		AMonster* Monster = Cast<AMonster>(m_HitResult.Actor);
		APawn* PlayerPawn = Cast<APawn>(GetOwner());
		if (Monster == nullptr || PlayerPawn == nullptr)
			return;

		AController* PlayerController = Cast<APlayerController>(PlayerPawn->GetController());
		if (PlayerController == nullptr)
			return;

		float Damage = m_Damage;
		bool IsHeadShot = m_HitResult.BoneName.ToString() == FString("head");
		if (IsHeadShot)
			Damage *= m_HeadDamageRate;

		UE_LOG(LogTemp, Log, TEXT("%s"), *m_HitResult.BoneName.ToString());

		UGameplayStatics::ApplyDamage(Monster, Damage, PlayerController, this, UDamageType::StaticClass());

		SpawnDamageText(m_HitResult, Damage, IsHeadShot);
	}
}