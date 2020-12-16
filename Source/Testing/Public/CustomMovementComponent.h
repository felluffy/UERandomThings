// Fill out your copyright notice in the Description page of Project Settings.
//https://github.com/Deema35/Climbing-Movement-Component
//Entire credits goes there, rewrote as it wouldn't work itself


#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Curves/CurveFloat.h"
#include "CustomMovementComponent.generated.h"


/**
 * 
 */
//
//enum class ECustomMovementPawnModeType : uint8;
//class FCustomMovementPawnModeBase;

UCLASS()
class TESTING_API UCustomMovementComponent : public UCharacterMovementComponent
{
	friend class FCustomMovementPawnModeRun;
	friend class FCustomMovementPawnModeClimb;
	GENERATED_BODY()

public:

	//UCustomMovementComponent(const FObjectInitializer& ObjectInitializer);
	UCustomMovementComponent();

	//UFUNCTION(BlueprintCallable, Category = CustomMovement)
	//void SetMovementMode(ECustomMovementPawnModeType MovementMode);

	/*UFUNCTION(BlueprintPure, Category = CustomMovement)
	ECustomMovementPawnModeType GetCurrentCustomMomvementMode() const { return CurrentMovementMode; }*/

public:
	//Height from location.z where character can grab surface
	UPROPERTY(Category = CustomMovement, EditDefaultsOnly, BlueprintReadWrite)
	int32 ClimbingSnatchHeight = 70;

	//offset between top of climbing surface and character location.z 
	UPROPERTY(Category = CustomMovement, EditAnywhere, BlueprintReadWrite)
	int32 ClimbDeltaZ = -85;

	//offset between character and wall
	UPROPERTY(Category = CustomMovement, EditAnywhere, BlueprintReadWrite)
	int32 ClimbDeltaWallOffset = 45;

	//velocity of climb movement
	UPROPERTY(Category = CustomMovement, EditAnywhere, BlueprintReadWrite)
	float ClimbVelocity = 150;

	//velocity jump from climb state
	UPROPERTY(Category = CustomMovement, EditAnywhere, BlueprintReadWrite)
	float ClimbJumpVelocity = 600;

	//velocity lift on plane
	UPROPERTY(Category = CustomMovement, EditAnywhere, BlueprintReadWrite)
	float ClimbLiftVelocity = 500;

	//helight from caharacter location z where jump will work
	UPROPERTY(Category = CustomMovement, EditAnywhere, BlueprintReadWrite)
	int32 JumpOverObjectDeltaHeight = 0;

	//minimum vlocity lifting on objects
	UPROPERTY(Category = CustomMovement, EditAnywhere, BlueprintReadWrite)
	int32 JumpOverObjectsVelocity = 500;

	//offset from wall when wall run
	UPROPERTY(Category = CustomMovement, EditAnywhere, BlueprintReadWrite)
	int32 WallOffset = 50;

	//multiplier input vecotr when character can run on wall
	UPROPERTY(Category = CustomMovement, EditAnywhere, BlueprintReadWrite)
	float WallRunFallGravityScale = .15;

	//Multiplier input vector when character runs on wall
	UPROPERTY(Category = CustomMovement, EditAnywhere, BlueprintReadWrite)
	int32 WallRunInputVelocity = 3;

	//Multiplier input vector when charecter run on wall
	UPROPERTY(Category = CustomMovement, EditAnywhere, BlueprintReadWrite)
	int32 WallRunCameraRoll = 20;

	//veclocity of jump on wall
	UPROPERTY(Category = CustomMovement, EditDefaultsOnly, BlueprintReadWrite)
	float WallRunJumpOnWallZVelocity = 500;

	//Velocyty of jump from wall forward
	UPROPERTY(Category = CustomMovement, EditDefaultsOnly, BlueprintReadWrite)
	float WallRunJumpForwardVelocity = 600;

	//velocity of jump from wall up
	UPROPERTY(Category = CustomMovement, EditDefaultsOnly, BlueprintReadWrite)
	float WallRunJumpUpVelocity = 500;
	
	//Offset from rope of zip line
	UPROPERTY(Category = CustomMovement, EditDefaultsOnly, BlueprintReadWrite)
	int32 ZipLineDeltaZ = 0;

	UPROPERTY(Category = CustomMovement, EditDefaultsOnly, BlueprintReadWrite)
	float ZipLineVelocity = 800;

	UPROPERTY(Category = CustomMovement, EditDefaultsOnly, BlueprintReadWrite)
	float ZipLineJumpVelocity = 600;

	UPROPERTY(Category = CustomMovement, EditDefaultsOnly, BlueprintReadWrite)
	float LadderMoveVelocity = 500;

	UPROPERTY(Category = CustomMovement, EditDefaultsOnly, BlueprintReadWrite)
	float DistanceToLadder = 20;

	UPROPERTY(Category = CustomMovement, EditDefaultsOnly, BlueprintReadWrite)
	float InclinedSlideAngle = 35;

	UPROPERTY(Category = CustomMovement, EditDefaultsOnly, BlueprintReadWrite)
	float InclinedSlideVeocityForward = 400;

	UPROPERTY(Category = CustomMovement, EditDefaultsOnly, BlueprintReadWrite)
	float InclinedSlideVelocityRight = 200;

	UPROPERTY(Category = CustomMovement, EditDefaultsOnly, BlueprintReadWrite)
	float InclinedJumpVelocity = 600;

	UPROPERTY(Category = CustomMovement, EditDefaultsOnly, BlueprintReadWrite)
	float RoundingTheCornerVelocity = 300;

	UPROPERTY(Category = CustomMovement, EditDefaultsOnly, BlueprintReadWrite)
	float RoundingTheCornerRotationVelocity = 400;

	//velocity of jump near wall
	UPROPERTY(Category = CustomMovement, EditDefaultsOnly, BlueprintReadWrite)
	float UnderWallJumpZVelocity = 700;

	UPROPERTY(Category = CustomMovement, EditDefaultsOnly, BlueprintReadWrite)
	float UnderWallJumpFromWallForwardVelocity = 500;

	UPROPERTY(Category = CustomMovement, EditDefaultsOnly, BlueprintReadWrite)
	float UnderWallJumpFromwallZVelocity = 300;

	//distance before wall where walljump can work
	UPROPERTY(Category = CustomMovement, EditDefaultsOnly, BlueprintReadWrite)
	float WallJumpDistance = 230;

	//Run Curve disabled

	//velocity runtime curve slide
	UPROPERTY(Category = CustomMovement, EditDefaultsOnly, BlueprintReadWrite)
	FRuntimeFloatCurve SlideVelocityCurve;

	//Threshold velocity when character can slide
	UPROPERTY(Category = CustomMovement, EditDefaultsOnly, BlueprintReadWrite)
	float SlideThresHold = 0.75;
public:
	virtual void BeginPlay() override;

	virtual bool DoJump(bool bReplayingMoves) override;

	virtual float GetMaxSpeed() const override;

	virtual void TickComponent
	(
		float DeltaTime,
		enum ELevelTick TickType,
		FActorComponentTickFunction * ThisTickFunction
	) override;
	
	void MoveTo(FVector Delta, FRotator NewRotation, bool CheckCollision);
	
	void YawCameraSet(int NewYaw);
	
	void AddYawCamera(int DeltaYaw);

	void RollCameraSet(int NewRoll);

	//FClimbingPawnModeBase& GetMode(EClimbingPawnModeType ModeType) { return ModeStorage->Get(ModeType); }
private:
	float CurrentRunTime = 0;
	float MaxRunTime = 0;
	float MinRunVelcoity = 0;
private: 
	//ECustomMovementPawnModeType CurrentMovementMode;
	FTimerHandle SlideTimerHandle;
	class ACharacter* OwningCharacter;
};
