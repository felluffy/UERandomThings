// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponBase.h"
#include <ctime>
#include "GameFramework/Actor.h"
#include "Math/RandomStream.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "DisplayDebugHelpers.h"
#include "Engine.h"
#include "MainCharacterBase.h"

AWeaponBase::AWeaponBase()
{
	PrimaryActorTick.bCanEverTick = false;
	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>("WeaponMesh");	
	MuzzleLocation = CreateDefaultSubobject<USceneComponent>("MuzzleLocation");
	MuzzleLocation->AttachToComponent(WeaponMesh, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, true), MuzzleSocketName);
	WeaponMesh->AttachToComponent(RootComponent, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, true));
	Type = WeaponType::Full;
	RS.Initialize(time(0));
}

void AWeaponBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


void AWeaponBase::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Warning, TEXT("Init done"));
}

void AWeaponBase::Fire()
{
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Fire"));
	OnFire();
	if (IsMagEmpty())
		return;

	if (CurrentStatus == Status::Idle || CurrentStatus == Status::Shooting)
	{
		CurrentStatus = Status::Shooting;
		OnFire();
	}
}

void AWeaponBase::OnDrop()
{
	
}

void AWeaponBase::OnPickUp()
{
	
}

void AWeaponBase::FindWhereToShootFrom(FVector &SpawnLocation, FRotator &SpawnRotation)
{
	auto Velocity = GetOwner()->GetVelocity();
	auto Speed = Velocity.Size();

	FVector SpreadAdjustment = FVector::ZeroVector;
	if (MuzzleLocation)
	{
		if (!bZoomedIn)
			SpreadAdjustment = RS.VRandCone(GetOwner()->GetActorForwardVector(), Spread * Speed, Spread * Speed);
		else
			SpreadAdjustment = RS.VRandCone(GetOwner()->GetActorForwardVector(), Spread * Speed / 10, Spread * Speed / 10);
	}
	SpawnRotation = UKismetMathLibrary::MakeRotFromX(SpreadAdjustment);
	SpawnLocation = ((MuzzleLocation != nullptr) ? MuzzleLocation->GetComponentLocation() : GetActorLocation());
}

void AWeaponBase::OnFire()
{
	bRecoil = false;
	FRotator SpawnRotation;
	FVector SpawnLocation;
	FindWhereToShootFrom(SpawnLocation, SpawnRotation);
	FActorSpawnParameters ActorSpawnParams;
	ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;
	ActorSpawnParams.Instigator = Cast<APawn>(GetOwner());
	ActorSpawnParams.Owner = GetOwner();
	PlayAfterEffects();
	HandleProjectile(ActorSpawnParams, SpawnLocation, SpawnRotation);
	HandleRecoil();
	ConsumeAmmo(NumberOfAmmoPerShotAsMode[0]);
	CurrentStatus = Status::Idle;
}

void AWeaponBase::HandleProjectile(FActorSpawnParameters &ActorSpawnParams, FVector &SpawnLocation, FRotator &SpawnRotation)
{
	TArray<FHitResult> HitResults;
	FVector LineTraceEnd = SpawnLocation + (SpawnRotation.Vector() * WeaponRange);
	UKismetSystemLibrary::LineTraceMulti(GetWorld(), SpawnLocation, LineTraceEnd, ETraceTypeQuery::TraceTypeQuery1, true, (TArray<AActor*>{this, GetOwner()}), EDrawDebugTrace::ForDuration, HitResults, true, FColor(FColor::Red), FColor::Green, 10.0f);
	float WeaponDamageCopy = WeaponDamage;
	for (auto &it : HitResults)
	{
		float DamageToApply = CalculateDamageByBodyPart(it.BoneName, WeaponDamageCopy);
		UGameplayStatics::ApplyPointDamage(it.GetActor(), DamageToApply, GetActorLocation(), it, GetOwner()->GetInstigatorController() == nullptr ? nullptr : GetOwner()->GetInstigatorController(), this, UDamageType::StaticClass());
		WeaponDamageCopy = CalculateDamageAfterPenetration(UPhysicalMaterial::DetermineSurfaceType(it.PhysMaterial.Get()), WeaponDamageCopy);
		PlayImpactParticles(UPhysicalMaterial::DetermineSurfaceType(it.PhysMaterial.Get()));
	}
}

float AWeaponBase::CalculateDamageByBodyPart(FName HitBone, float DefaultDamage)
{
	return DefaultDamage;
}

float AWeaponBase::CalculateDamageAfterPenetration(EPhysicalSurface, float DefaultDamage)
{
	return DefaultDamage;
}

void AWeaponBase::OnZoomIn()
{
	
}

void AWeaponBase::OnZoomOut()
{

}

void AWeaponBase::Reload()
{
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Reload Triggered"));
	OnReload();
}

void AWeaponBase::OnReload()
{
	if (CurrentStatus != Status::Reloading)
	{
		StopFire();
		///@TODO Handle if owner is allowed to swap weapons during reloading 
	
		if (CurrentAmmoInMagazine == MagazineCapacity)
			return;
		CurrentStatus = Status::Reloading;

		float ReloadAnimationDuration = 0.0f;
		ReloadAnimationDuration = PlayWeaponAnimation(ReloadAnimation);
		if(ReloadAnimationDuration <= 0.0f)
			ReloadAnimationDuration = DefaultReloadTime;

		GetWorldTimerManager().SetTimer(ReloadTimer, this, &AWeaponBase::Reload, ReloadAnimationDuration, false);
		
	}
}

void AWeaponBase::HandleReload()
{
	AmmoCount += CurrentAmmoInMagazine;
	if(AmmoCount >= MagazineCapacity)
		CurrentAmmoInMagazine = MagazineCapacity;
	else
		CurrentAmmoInMagazine = AmmoCount % (MagazineCapacity + 1);
	AmmoCount -= CurrentAmmoInMagazine;

	//possibly change it to server/client from the sounds 
	if(ReloadSound.Num() > 0)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), ReloadSound[FMath::RandRange(0, ReloadSound.Num())], GetActorLocation());
	}
	CurrentStatus = Status::Idle;

	//Check to go here if upon reload should swapping be disabled
}

void AWeaponBase::Equip()
{

}

void AWeaponBase::OnEquip()
{
	this->SetActorTickEnabled(true);
	WeaponMesh->SetHiddenInGame(false);
	WeaponMesh->SetCollisionResponseToAllChannels(ECR_MAX);
	WeaponMesh->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
	OverlapSphere->SetCollisionResponseToAllChannels(ECR_Overlap);
	OverlapSphere->SetCollisionResponseToChannel(ECC_Camera, ECR_Overlap);
	OverlapSphere->SetCollisionResponseToChannel(ECC_Visibility, ECR_Overlap);
	CurrentStatus = Status::Idle;
	AttachMeshToPawn();
}

void AWeaponBase::UnEquip()
{

}

void AWeaponBase::OnUnEquip()
{
	this->SetActorTickEnabled(false);
	WeaponMesh->SetHiddenInGame(true);
	WeaponMesh->SetCollisionResponseToAllChannels(ECR_Overlap);
	OverlapSphere->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	OverlapSphere->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	OverlapSphere->SetCollisionResponseToChannel(ECC_Visibility, ECR_Ignore);
	StopFire();
}

void AWeaponBase::DrawWeaponCrossHair(bool ShouldDraw = true)
{
	
}

void AWeaponBase::StartFire()
{
	if(Type == WeaponType::Full)
	{
		float FirstDelay = FMath::Max(fLastFireTime + fIntervalShootingTime - GetWorld()->TimeSeconds, 0.0f);
		GetWorldTimerManager().SetTimer(FireTimer, this, &AWeaponBase::Fire, fIntervalShootingTime, true, FirstDelay);
	}
	else
	{
		Fire();
	}
}


void AWeaponBase::StopFire()
{
	GetWorldTimerManager().ClearTimer(FireTimer);
	bRecoil = false;
}

void AWeaponBase::AttachMeshToPawn()
{

}

void AWeaponBase::ConsumeAmmo(int32 NumberOfAmmoToConsume)
{
	CurrentAmmoInMagazine -= NumberOfAmmoToConsume;
	if(CurrentAmmoInMagazine <= 0)
		CurrentStatus = Status::ToReload;
}

bool AWeaponBase::IsMagEmpty()
{
	return (CurrentAmmoInMagazine <= 0) ? true : false;
}

void AWeaponBase::AttachAttachment(UActorComponent* AttachToAdd)
{
	
}

void AWeaponBase::OnPressActionButton()
{
	StartFire();
}

void AWeaponBase::OnPressSecondaryButton()
{
	OnZoomIn();
}

void AWeaponBase::OnPressMainButton()
{
	StartFire();
}

void AWeaponBase::OnReleaseActionButton()
{
	
}

void AWeaponBase::OnReleaseSecondaryButton()
{
	
}

void AWeaponBase::OnReleaseMainButton()
{
	StopFire();
}

void AWeaponBase::PlayAfterEffects()
{
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Play impact particles"));
	if (FireSound.Num() > 0)
	{
		auto SoundToPlay = FireSound[FMath::RandRange(0, FireSound.Num())];
		MakeNoise(FireSoundScale, GetOwner()->GetInstigator(), GetActorLocation(), FireSoundRange);
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), SoundToPlay, GetActorLocation(), 1, 1, 0, FireSoundAttenuation );
	}
	if (MuzzleParticle.Num() > 0)
	{
		auto MuzzleParticleToEmit = MuzzleParticle[FMath::RandRange(0, MuzzleParticle.Num())];
		UGameplayStatics::SpawnEmitterAttached(MuzzleParticleToEmit, WeaponMesh, MuzzleSocketName);
	}
	if (FireShake != NULL)
	{

		//@TODO Fix up play shake through controller directly
		// if (GetWorld()->GetFirstPlayerController() != NULL)
		// 	GetWorld()->GetFirstPlayerController()->PlayerCameraManager->PlayCameraShake(FireShake, fFireShakeAlpha, ECameraAnimPlaySpace::	CameraLocal);
		auto OwnerController = Cast<APlayerController>(GetInstigatorController());
		if(OwnerController)
			OwnerController->PlayerCameraManager->PlayCameraShake(FireShake, fFireShakeAlpha, ECameraAnimPlaySpace::CameraLocal);
	}
	if(!bZoomedIn)
		PlayWeaponAnimation(FireAnimation);
	else 
		PlayWeaponAnimation(FireAnimationSights);
}

void AWeaponBase::PlayImpactParticles(EPhysicalSurface SurfaceType)
{
	
}

void AWeaponBase::HandleRecoil()
{

}

float AWeaponBase::PlayWeaponAnimation(const FWeaponAnim& Animation, float InPlayRate)
{
	float Duration = 0.0f;
	auto Owner_ = Cast<AMainCharacterBase>(GetOwner()); 
	if (GetOwner())
	{
		
		if (Animation.Pawn3P)
		{
			Duration = Owner_->PlayAnimMontageOnMesh(Animation.Pawn3P, InPlayRate, NAME_None, false);
		}
		UAnimMontage* AnimToPlay = Animation.Pawn3P;
		if (AnimToPlay)
		{
			Duration = Owner_->PlayAnimMontageOnMesh(AnimToPlay, InPlayRate, NAME_None, true);
		}
	}

	return Duration;
}

void AWeaponBase::StopWeaponAnimation(const FWeaponAnim& Animation)
{
	auto Owner_ = Cast<AMainCharacterBase>(GetOwner()); 
	if (Owner_)
	{
		Owner_->StopAllMontage();
	}
}

void AWeaponBase::OnThrowDropAddImpulse(FVector Impulse, FVector Location, FName BoneName)
{
	WeaponMesh->AddImpulseAtLocation(Impulse, Location, BoneName);
}
