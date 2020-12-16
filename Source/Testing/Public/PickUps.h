// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TInteractable.h"
#include "PickUps.generated.h"


UCLASS()
class TESTING_API APickUps : public ATInteractable
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APickUps();
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Pickups)
	class USphereComponent* OverlapSphere;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Pickups)
	class UParticleSystemComponent* OnPickUpParticle;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Pickups)
	class UParticleSystemComponent* OnSpawnParticle;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Pickups)
	class UParticleSystemComponent* DefaultParticle;

//properties
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Pickups)
	float RespawnTimer;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Pickups)
	float MovementXAxis;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Pickups)
	float MovementYAxis;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Pickups)
	float MovementZAxis;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Pickups)
	float RotationXAxis;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Pickups)
	float RotationYAxis;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Pickups)
	float RotationZAxis;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Pickups)
	bool bOnceUse = false;

protected:
	//void ChangeTransforms();
public:
	UFUNCTION(BlueprintCallable, Category = Pickups)
	virtual void OnPickUp();
	
	UFUNCTION(BlueprintCallable, Category = Pickups)
	virtual void OnDrop();

	//UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = Pickups)
	//virtual void OnRespawn();

	//virtual bool CanBePickedUp(class AActor* ) const;

	//UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = Pickups)
	//virtual void RespawnPickup();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
