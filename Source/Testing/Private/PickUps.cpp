// Fill out your copyright notice in the Description page of Project Settings.


#include "PickUps.h"

#include "MainCharacterBase.h"
#include "Components/SphereComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/KismetSystemLibrary.h"
// Sets default values
APickUps::APickUps()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	OverlapSphere = CreateDefaultSubobject<USphereComponent>("CollisionComponent");
	RootComponent = OverlapSphere;
	//OverlapSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	//OverlapSphere->SetCollisionResponseTo
}

void APickUps::Overlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	///@TODO Draw Widget as prompt
}

// Called when the game starts or when spawned
void APickUps::BeginPlay()
{
	Super::BeginPlay();
	OverlapSphere->OnComponentBeginOverlap.AddDynamic( this, &APickUps::Overlap );	
}

// Called every frame
void APickUps::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APickUps::OnPickUp()
{
	//add to inventory of owner //@TODO
}

void APickUps::OnDrop()
{
	FVector OwnerLocation = GetOwner()->GetActorLocation();
	OwnerLocation += GetOwner()->GetActorForwardVector() * 25;
	//DetachFromOwner();

	//SpawnParticles, aftereffects again after heres

}

void APickUps::OnEquip()
{
	
}

void APickUps::OnUnEquip()
{
	
}

void APickUps::ThrowUponDrop(bool ShouldThrow, FName SocketAttachmentPoint)
{
	if(!ShouldThrow)
		return;
	auto Owner_ = Cast<AMainCharacterBase>(GetOwner());
	FVector ThrowFromLocation = Owner_ != nullptr ? Owner_->GetMesh()->GetSocketLocation(SocketAttachmentPoint) : GetActorLocation();
	FVector ThrowDirection = GetActorForwardVector() * MaxThrowDistance; //readjust to allow throwing distance based upon how much the trigger is held
	FHitResult Hit;
	UKismetSystemLibrary::LineTraceSingle(GetWorld(), ThrowFromLocation, ThrowDirection, ETraceTypeQuery::TraceTypeQuery1, true, TArray<AActor*>({this}), EDrawDebugTrace::ForDuration, Hit, true,  FColor::Red, FColor::Green, 5);
	if(!Hit.bBlockingHit)
	{
		OnThrowDropAddImpulse((ThrowDirection - ThrowFromLocation) * MaxThrowDistance, ThrowFromLocation, "");
	}
	else
		return;
	
	
}

void APickUps::OnThrowDropAddImpulse(FVector Impulse, FVector Location, FName BoneName)
{
}

void APickUps::ClearOwnerUponDrop()
{
	
}

void APickUps::AddOwnerUponPickedUp(ACharacter* CharacterToAdd)
{
}
