// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomMovementComponent.h"
#include "GameFramework/Character.h"
#include "Engine/World.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include <vector>
#include "Components/SplineComponent.h"

//
//UCustomMovementComponent::UCustomMovementComponent(const class FObjectInitialize& ObjectInitializer)// : Super(ObjectInitializer)// mode storage something, 
//{
//	MaxWalkSpeed = 500;
//	NavAgentProps.bCanCrouch = true;
//	//Super::bOrientToMovement = true;
//
//
//}

UCustomMovementComponent::UCustomMovementComponent()
{

}

void UCustomMovementComponent::BeginPlay()
{
	Super::BeginPlay();

}

void UCustomMovementComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction * ThisTickFunction)
{
	//if(!PawnOwner)
}

bool UCustomMovementComponent::DoJump(bool bReplayingMoves)
{
	return Super::DoJump(bReplayingMoves);
	return false;
	//bool ReturnValue;
	//return Super::DoJump(bReplayingMoves);
	////if (ModeStorage->Get(CurrentClimbingMode).DoJump(bReplayingMoves, ReturnValue))
	////{
	////	return Super::DoJump(bReplayingMoves);
	////}
	////else
	////{
	////	return ReturnValue;
	////}
}

float UCustomMovementComponent::GetMaxSpeed() const
{
	float MaxSpeed;

	MaxSpeed = Super::GetMaxSpeed();

	return MaxSpeed;
}

void UCustomMovementComponent::MoveTo(FVector Delta, FRotator NewRotation, bool CheckCollision)
{
	if (CheckCollision)
	{
		FHitResult Hit;
		SafeMoveUpdatedComponent(Delta, NewRotation, true, Hit);
		if (Hit.IsValidBlockingHit())
		{
			SlideAlongSurface(Delta, 1.f - Hit.Time, Hit.Normal, Hit, false);
		}
	}
	else
	{
		GetPawnOwner()->AddActorWorldOffset(Delta);
		GetPawnOwner()->SetActorRotation(NewRotation);
	}
}

void UCustomMovementComponent::RollCameraSet(int NewRoll)
{
	FRotator Rot = GetWorld()->GetFirstPlayerController()->GetControlRotation();
	Rot.Roll = NewRoll;
	GetWorld()->GetFirstPlayerController()->SetControlRotation(Rot);

}


void UCustomMovementComponent::YawCameraSet(int NewYaw)
{
	FRotator Rot = GetWorld()->GetFirstPlayerController()->GetControlRotation();
	Rot.Yaw = NewYaw;
	GetWorld()->GetFirstPlayerController()->SetControlRotation(Rot);
}

void UCustomMovementComponent::AddYawCamera(int DeltaYaw)
{
	FRotator Rot = GetWorld()->GetFirstPlayerController()->GetControlRotation();

	Rot.Yaw += DeltaYaw;

	GetWorld()->GetFirstPlayerController()->SetControlRotation(Rot);
}