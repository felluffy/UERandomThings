// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WeaponBase.h"
#include "WeaponProjectileBased.generated.h"

/**
 * 
 */
UCLASS()
class TESTING_API AWeaponProjectileBased : public AWeaponBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	bool ChangeProjectile(int32 Index);

protected:
	virtual void HandleProjectile(FActorSpawnParameters &ActorSpawnParams, FVector &SpawnLocation, FRotator &SpawnRotation) override;

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
	TArray<TSubclassOf<class ATProjectile>> ProjectileTypes;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class ATProjectile> ProjectileClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
	float LaunchSpeed;

	//UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
	//float 
};
