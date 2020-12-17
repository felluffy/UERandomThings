// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MainCharacterBase.generated.h"

UCLASS()
class TESTING_API AMainCharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMainCharacterBase(const FObjectInitializer& ObjectInitializer);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

//Components
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FP_CameraComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* FP_SpringArm;
	//Can add third person
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* TP_CameraComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* TP_SpringArm;

//Initialize on beginplay
private:
	void SetupCameras();
		
//Locomotion
private:
	UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class  UCustomMovementComponent* MovementComponent;

protected:
	UFUNCTION(BlueprintCallable, Category = "Movement")
	void MoveForward(float value);

	UFUNCTION(BlueprintCallable, Category = "Movement")
	void MoveSide(float value);

	UFUNCTION(BlueprintCallable, Category = "Movement")
	void BeginCrouch();

	UFUNCTION(BlueprintCallable, Category = "Movement")
	void EndCrouch();

	UFUNCTION(BlueprintCallable, Category = "Movement")
	void SlideForward();

	UFUNCTION(BlueprintCallable, Category = "Movement")
	void BeginProne();

	UFUNCTION(BlueprintCallable, Category = "Movement")
	void EndProne();

	UFUNCTION(BlueprintCallable, Category = "Movement")
	void TiltSideways();

	void Jump() override;
	void StopJumping() override;

protected:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	float MaxWalkSpeed;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	float MaxRunSpeed;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	float BackWardMovementPenalty;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	float MaxProneSpeed;

//camera
protected:
//normalized turn rates
	UFUNCTION(BlueprintCallable, Category = "Camera")
	void TurnAtRate(float Rate);

	UFUNCTION(BlueprintCallable, Category = "Camera")
	void LookUpAtRate(float Rate);

	virtual void GetActorEyesViewPoint(FVector& Location, FRotator& Rotation) const override;
	void GetPerceptionLocationAndRotation(FVector& Location, FRotator& Rotation) const;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	float BaseTurnRate;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	float BaseLookUpRate;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	FName EyeSocketName = "EyeSocket";

protected:
	///StoreCurrentTrace
	void StoreCurrentTrace();
	FHitResult CurrentFrameTrace;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	float DefaultTraceDistance;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	float DefaultBoxTraceDistance;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	float DimensionSquare;

	//Invenetory related and action pressses 
protected:
	virtual void OnPressedActionButton();

	virtual void SwapCamera(class UCameraComponent* OldCamera, class UCameraComponent* NewCamera);

//Animation assets
public:
	UFUNCTION(BlueprintCallable, Category = "Animations")
	float PlayAnimMontageOnMesh(class UAnimMontage* AnimMontage, float InPlayRate = 1.f, FName StartSectionName = NAME_None, bool UseFP = false); //for first person

	float PlayAnimMontage(class UAnimMontage* AnimMontage, float InPlayRate = 1.f, FName StartSectionName = NAME_None) override;

	UFUNCTION(BlueprintCallable, Category = "Animations")
	void StopAllMontage();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};

