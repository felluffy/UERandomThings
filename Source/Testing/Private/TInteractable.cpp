// Fill out your copyright notice in the Description page of Project Settings.


#include "TInteractable.h"

// Sets default values
ATInteractable::ATInteractable()
{	
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	

}

// Called when the game starts or when spawned
void ATInteractable::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATInteractable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATInteractable::OnFocusSpawnWidget()
{

}

void ATInteractable::OnFocusLostDisappear()
{

}