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

}

void AWeaponBase::Equip()
{

}

void AWeaponBase::OnEquip()
{

}

void AWeaponBase::UnEquip()
{

}

void AWeaponBase::OnUnEquip()
{

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

bool AWeaponBase::IsMagEmpty()
{
	return true;
}

void AWeaponBase::AttachAttachment(UActorComponent* AttachToAdd)
{

}

void AWeaponBase::PlayAfterEffects()
{
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Play impact particles"));
}

void AWeaponBase::PlayImpactParticles(EPhysicalSurface SurfaceType)
{
	
}

void AWeaponBase::HandleRecoil()
{

}
