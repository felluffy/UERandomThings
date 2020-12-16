// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TInteractable.generated.h"

UCLASS()
class TESTING_API ATInteractable : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATInteractable();

public:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	TArray<TSubclassOf<class UUserWidget>> WidgetsToShow;

	virtual void OnFocusSpawnWidget();
	virtual void OnFocusLostDisappear();
};
