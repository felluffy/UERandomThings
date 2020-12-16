// Fill out your copyright notice in the Description page of Project Settings.


#include "MainCharacterBase.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Components/InputComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Engine/World.h"
#include "Camera/CameraComponent.h" 	
#include "CustomMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "TimerManager.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
AMainCharacterBase::AMainCharacterBase(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer.SetDefaultSubobjectClass<UCustomMovementComponent>(ACharacter::CharacterMovementComponentName))
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	FP_SpringArm = CreateDefaultSubobject<USpringArmComponent>("CameraBoom");
	FP_CameraComponent = CreateDefaultSubobject<UCameraComponent>("FP_Camera");

	TP_SpringArm = CreateDefaultSubobject<USpringArmComponent>("TP_CameraBoom");
	TP_CameraComponent = CreateDefaultSubobject<UCameraComponent>("TP_Camera");

	FP_SpringArm->SetupAttachment(RootComponent);
	FP_CameraComponent->SetupAttachment(FP_SpringArm);
	
	TP_SpringArm->SetupAttachment(RootComponent);
	TP_CameraComponent->SetupAttachment(TP_SpringArm);


	MovementComponent = Cast<UCustomMovementComponent>(Super::GetMovementComponent());
	//MovementComponent = FindComponentByClass<UCharacterMovementComponent>();
	

}

// Called when the game starts or when spawned
void AMainCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	SetupCameras();
}

// Called every frame
void AMainCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	StoreCurrentTrace();
}

void AMainCharacterBase::SetupCameras()
{	
	if (FP_SpringArm != NULL && FP_CameraComponent != NULL)
	{
		FP_SpringArm->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, EyeSocketName);
		FP_SpringArm->TargetArmLength = 0.0f;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Camera or spring arm hasnot been instantiated"));
		return;
	}

	if (TP_SpringArm != NULL && TP_CameraComponent != NULL)
	{
		TP_CameraComponent->Deactivate();
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Third person camera or spring arm hasnot been instantiated"));
		return;
	}
}

void AMainCharacterBase::MoveForward(float Value)
{
	if (Value < 0.0f)
	{
		Value /= BackWardMovementPenalty;
	}
	AddMovementInput(GetActorForwardVector(), Value);
}


void AMainCharacterBase::MoveSide(float Value)
{
	Value /= 1.25;
	AddMovementInput(GetActorRightVector(), Value);
}

void AMainCharacterBase::BeginCrouch()
{
	Crouch();
}

void AMainCharacterBase::EndCrouch()
{
	UnCrouch();
}	

void AMainCharacterBase::SlideForward()
{

}

void AMainCharacterBase::BeginProne()
{

}

void AMainCharacterBase::EndProne()
{

}

void AMainCharacterBase::Jump()
{
	Super::Jump();
}

void AMainCharacterBase::StopJumping()
{
	Super::StopJumping();
}
void AMainCharacterBase::TiltSideways()
{

}

void AMainCharacterBase::TurnAtRate(float Rate)
{

}

void AMainCharacterBase::LookUpAtRate(float Rate)
{

}

void AMainCharacterBase::GetActorEyesViewPoint(FVector& Location, FRotator& Rotation) const
{
	GetPerceptionLocationAndRotation(Location, Rotation);
}

void AMainCharacterBase::GetPerceptionLocationAndRotation(FVector& Location, FRotator& Rotation) const
{
	GetMesh()->GetSocketWorldLocationAndRotation(EyeSocketName, Location, Rotation);
}

void AMainCharacterBase::StoreCurrentTrace()
{
	FVector LineTraceStart;
	FRotator ActorRotation;
	GetActorEyesViewPoint(LineTraceStart, ActorRotation);
	FVector LineTraceEnd = LineTraceStart + (ActorRotation.Vector() * DefaultTraceDistance);
	FVector BoxTraceEnd = LineTraceStart + (ActorRotation.Vector() * DefaultBoxTraceDistance);
	FHitResult Hit;
	UKismetSystemLibrary::LineTraceSingle(GetWorld(), LineTraceStart, LineTraceEnd, ETraceTypeQuery::TraceTypeQuery1, false, { this }, EDrawDebugTrace::ForOneFrame, Hit, true);
	TArray<FHitResult> Hits;
	UKismetSystemLibrary::BoxTraceMulti(GetWorld(), LineTraceStart, BoxTraceEnd, { 10,10,10 }, { 0,0,0 }, ETraceTypeQuery::TraceTypeQuery1, false, { this }, EDrawDebugTrace::ForOneFrame, Hits, true);

}

void AMainCharacterBase::OnPressedActionButton()
{

}

void AMainCharacterBase::SwapCamera(UCameraComponent* OldCamera, UCameraComponent* NewCamera)
{
	auto Con = GetController();
	if (Con != NULL && NewCamera == NULL)
	{
		UE_LOG(LogTemp, Error, TEXT("New camera wasn't passed"));
		return;
	}
	auto PlayerCon = Cast<APlayerController>(Con);
	

	OldCamera->Deactivate();
	NewCamera->Activate();
	if (PlayerCon != NULL)
	{
		//PlayerCon->Set
	}
}

// Called to bind functionality to input
void AMainCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("ActionButton", IE_Pressed, this, &AMainCharacterBase::OnPressedActionButton);


	//Locomotion related
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AMainCharacterBase::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &AMainCharacterBase::StopJumping);
	PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &AMainCharacterBase::BeginCrouch);
	PlayerInputComponent->BindAction("Crouch", IE_Released, this, &AMainCharacterBase::EndCrouch);
	PlayerInputComponent->BindAxis("MoveForward", this, &AMainCharacterBase::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AMainCharacterBase::MoveSide);
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AMainCharacterBase::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AMainCharacterBase::LookUpAtRate);
}

