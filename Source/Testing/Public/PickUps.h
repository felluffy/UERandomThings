// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TInteractable.h"
#include "GameFramework/Character.h"

#include "PickUps.generated.h"

USTRUCT(BlueprintType)
struct FInventoryAttributes
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Inventory)
	float Weight;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Inventory)
	int TopLeftBox;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Inventory)
	int RightOccupancy;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Inventory)
	int BottomOccupancy;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Inventory)
	class UTexture2D* ItemTexture;
};


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

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Pickups)
	bool bAllowedInInventory = true;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Pickups)
	FInventoryAttributes InventoryAttributes;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Pickups)
	float MaxThrowDistance;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Pickups)
	float ThrowMultiplier;



protected:
	//void ChangeTransforms();
public:
	UFUNCTION(BlueprintCallable, Category = Pickups)
	virtual void OnPickUp();
	
	UFUNCTION(BlueprintCallable, Category = Pickups)
	virtual void OnDrop();
	
	UFUNCTION(BlueprintCallable, Category = Pickups)
    virtual void OnEquip();
    	
    UFUNCTION(BlueprintCallable, Category = Pickups)
    virtual void OnUnEquip();

	UFUNCTION(BlueprintCallable, Category = Pickups)
    virtual void ThrowUponDrop(bool ShouldThrow = false, FName SocketAttachmentPoint = "");

	UFUNCTION(BlueprintCallable, Category = Pickups)
    virtual void OnThrowDropAddImpulse(FVector Impulse, FVector Location, FName BoneName);

	UFUNCTION(BlueprintCallable, Category = Pickups)
	inline FInventoryAttributes GetInventoryAttributes() const { return InventoryAttributes; }

	UFUNCTION(BlueprintCallable, Category = PickUps)
    virtual void ClearOwnerUponDrop();

	UFUNCTION(BlueprintCallable, Category = PickUps)
    virtual void AddOwnerUponPickedUp(class ACharacter* CharacterToAdd); 

	UFUNCTION(BlueprintCallable, Category = Input)
	virtual void OnPressMainButton() { return; } ;
	
	UFUNCTION(BlueprintCallable, Category = Input)
    virtual void OnPressSecondaryButton() { return; };
    
	UFUNCTION(BlueprintCallable, Category = Input)
    virtual void OnPressActionButton() { return; };

	UFUNCTION(BlueprintCallable, Category = Input)
    virtual void OnReleaseActionButton() { return; }

	UFUNCTION(BlueprintCallable, Category = Input)
    virtual void OnReleaseSecondaryButton() { return; }

	UFUNCTION(BlueprintCallable, Category = Input)
    virtual void OnReleaseMainButton() { return; }

	UFUNCTION(BlueprintCallable, Category = Input)
    virtual void ResetToDefaultState() { return; } //Zoomout, change firing modes and things

	UFUNCTION(BlueprintCallable, Category = Input)
    virtual bool CanBeSwapped() { return true; } //Zoomout, change firing modes and things

	UFUNCTION( )
	virtual void Overlap( UPrimitiveComponent *OverlappedComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult );
	
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
