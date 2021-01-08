// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WeaponBase.h"
#include "LaserWeapon.generated.h"

/**
 * 
 */
UCLASS()
class TESTING_API ALaserWeapon : public AWeaponBase
{
	GENERATED_BODY()
	virtual void HandleProjectile(FActorSpawnParameters &ActorSpawnParams, FVector &SpawnLocation, FRotator &SpawnRotation) override;
};
