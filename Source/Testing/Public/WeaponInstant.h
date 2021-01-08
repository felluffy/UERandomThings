// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WeaponBase.h"
#include "WeaponInstant.generated.h"

/**
 * 
 */
UCLASS()
class TESTING_API AWeaponInstant : public AWeaponBase
{
	GENERATED_BODY()

protected:
	virtual void HandleProjectile(FActorSpawnParameters &ActorSpawnParams, FVector &SpawnLocation, FRotator &SpawnRotation) override;

	
};
