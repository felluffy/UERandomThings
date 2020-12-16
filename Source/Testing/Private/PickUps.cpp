// Fill out your copyright notice in the Description page of Project Settings.


#include "PickUps.h"
#include "Components/SphereComponent.h"
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

// Called when the game starts or when spawned
void APickUps::BeginPlay()
{
	Super::BeginPlay();
	
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

}