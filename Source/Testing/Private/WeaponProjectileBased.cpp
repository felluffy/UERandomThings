// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponProjectileBased.h"
#include "TProjectile.h"
void AWeaponProjectileBased::HandleProjectile(FActorSpawnParameters &ActorSpawnParams, FVector &SpawnLocation, FRotator &SpawnRotation)
{
	//TArray<FHitResult> HitResults;
	//FVector LineTraceEnd = SpawnLocation + (SpawnRotation.Vector() * WeaponRange);
	//UKismetSystemLibrary::LineTraceMulti(GetWorld(), SpawnLocation, LineTraceEnd, ETraceTypeQuery::TraceTypeQuery1, true, (TArray<AActor*>{this, GetOwner()}), EDrawDebugTrace::ForDuration, HitResults, true, FColor(FColor::Red), FColor::Green, 10.0f);
	//float WeaponDamageCopy = WeaponDamage;
	//for (auto &it : HitResults)
	//{
	//	float DamageToApply = CalculateDamageByBodyPart(it.BoneName, WeaponDamageCopy);
	//	UGameplayStatics::ApplyPointDamage(it.GetActor(), DamageToApply, GetActorLocation(), it, GetOwner()->GetInstigatorController() == nullptr ? nullptr : GetOwner()->GetInstigatorController(), this, UDamageType::StaticClass());
	//	WeaponDamageCopy = CalculateDamageAfterPenetration(UPhysicalMaterial::DetermineSurfaceType(it.PhysMaterial.Get()), WeaponDamageCopy);
	//	PlayImpactParticles(UPhysicalMaterial::DetermineSurfaceType(it.PhysMaterial.Get()));
	//}

	//add to a pool later

	auto Projectile = GetWorld()->SpawnActor<ATProjectile>(ProjectileClass, SpawnLocation, SpawnRotation, ActorSpawnParams);
	if(Projectile)
		Projectile->SetProjectileProperties(false, LaunchSpeed, LaunchSpeed, 10000, 5000, true);

}


bool AWeaponProjectileBased::ChangeProjectile(int32 Index)
{
	return false;
}