// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Components/ActorComponent.h"
#include "Components/Widget.h"
#include "Engine/Texture2D.h"

#include "InventoryComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TESTING_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInventoryComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void AddItemToInventory(class APickUps* ItemToAdd);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
    class APickUps* DropItemInInventory(int32 Index);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	class APickUps* GetCurrentItem() const
	{
		//return ItemInHand;
		try
		{
			return ItemInHand;
		} catch (std::exception Ex)
		{
			UE_LOG(LogTemp, Error, TEXT("LLALALA Some error on inventory component"));
			return nullptr;
		}
	}
	
    class APickUps* RemoveItemFromInventory(int32 Index);
	
    class APickUps* RemoveItemFromInventory();
	
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	class APickUps* RemoveItemFromBP(bool RemoveFirst = false, int32 Index = 0);
	
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void NextItemInInventory();
	
	UFUNCTION(BlueprintCallable, Category = "Inventory")
    void PreviousItemInInventory();

	UFUNCTION(BlueprintCallable, Category = "Inventory")
    void ToggleInventory(bool State);
	
	UFUNCTION(BlueprintCallable, Category = "Inventory")
    void ShowInventory();

	UFUNCTION(BlueprintCallable, Category = "Inventory")
    void HideInventory();

	UFUNCTION(BlueprintCallable, Category = "Inventory")
    int32 GetInventoryItems() { return Items.Num(); };

private:
	void SetCurrentItem(class APickUps* PreviousItem, class APickUps* NewItem);
	
protected:
	TArray<class APickUps*> Items;

	class APickUps* ItemInHand;
	
private:
	bool bIsWidgetBeingShown = false;

	float CurrentOccupiedWeight;
protected:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Inventory")
	float MaxWeight;
	
	// UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Inventory")
	// class UWidget* InventoryWidget;
};
