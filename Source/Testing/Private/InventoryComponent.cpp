// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryComponent.h"
#include "Engine/World.h"
#include "PickUps.h"

// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UInventoryComponent::AddItemToInventory(APickUps* ItemToAdd)
{
	
	if(ItemToAdd != nullptr && ItemToAdd->GetInventoryAttributes().Weight + CurrentOccupiedWeight <= MaxWeight)
	{
		Items.AddUnique(ItemToAdd);
		ItemToAdd->OnPickUp();
		if(Items.Num() == 1)
			SetCurrentItem(nullptr, Items[0]);
		else
			ItemToAdd->OnUnEquip();
			
		

		if (GEngine)
        		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Added %s kewllzies"), *ItemToAdd->GetName());
	}
	else
		return;
}

APickUps* UInventoryComponent::DropItemInInventory(int32 Index)
{
	return RemoveItemFromInventory(Index);

}

void UInventoryComponent::SetCurrentItem(APickUps* PreviousItem, APickUps* NewItem)
{
	if(NewItem == nullptr)
		return;
	if(PreviousItem == nullptr || PreviousItem != NewItem)
		ItemInHand = NewItem;
	if(PreviousItem)
		PreviousItem->OnUnEquip();
	NewItem->OnEquip();		
}

APickUps* UInventoryComponent::RemoveItemFromInventory(int32 Index)
{
	if(Items.Num() <= Index)
		return nullptr;
	else
	{
		auto ToReturn = Items[Index];
		Items.RemoveAt(Index);
		ToReturn->OnUnEquip();
		ToReturn->OnDrop();
		ToReturn->ThrowUponDrop(true);
		NextItemInInventory();
		return ToReturn;
	}
}

APickUps* UInventoryComponent::RemoveItemFromInventory()
{
	if(GetCurrentItem() != nullptr)
	{
		int32 Index = 0;
		Items.Find(GetCurrentItem(), Index);
		auto ToReturn = Items[Index];
		Items.RemoveAt(Index);
		ToReturn->OnUnEquip();
		ToReturn->OnDrop();
		ToReturn->ThrowUponDrop(true);
		NextItemInInventory();
		return ToReturn;
	}
	else
		return nullptr;
}

APickUps* UInventoryComponent::RemoveItemFromBP(bool RemoveFirst, int32 Index)
{
	return nullptr;
}

void UInventoryComponent::NextItemInInventory()
{
	if(Items.Num() > 1 && GetCurrentItem()->CanBeSwapped())
	{
		GetCurrentItem()->ResetToDefaultState();
		int32 Index;
		Items.Find(GetCurrentItem(), Index);
		Index = (Index + 1) % Items.Num();
		SetCurrentItem(GetCurrentItem(), Items[Index]);
	}
}

void UInventoryComponent::PreviousItemInInventory()
{
	if(Items.Num() > 1 && GetCurrentItem()->CanBeSwapped())
	{
		GetCurrentItem()->ResetToDefaultState();
		int32 Index;
		Items.Find(GetCurrentItem(), Index);
		Index = (Index - 1 + Items.Num()) % Items.Num();
		SetCurrentItem(GetCurrentItem(), Items[Index]);
	}
}

void UInventoryComponent::ToggleInventory(bool State)
{
	///@TODO, show an interactable widget
}

void UInventoryComponent::ShowInventory()
{
	///@TODO, show an interactable widget
}

void UInventoryComponent::HideInventory()
{
	///@TODO, hide an interactable widget
}

