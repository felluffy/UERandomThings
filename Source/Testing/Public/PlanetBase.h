// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PlanetBase.generated.h"
//Need not be replicated
UCLASS()
class TESTING_API APlanetBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APlanetBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	APlanetBase *ParentStar = NULL;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float RevolveSpeed;
	

};
