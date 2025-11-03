// Fill out your copyright notice in the Description page of Project Settings.


#include "UInventoryManager.h"

#include "ADoor.h"
#include "Assignment2GameMode.h"
#include "Item.h"
#include "Characters/EnemyCharacter.h"
#include "Characters/PlayerCharacter.h"
#include "Components/SceneComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UInventoryManager::UInventoryManager()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	if (AActor* ActorToAttachTo = GetOwner())
	{
		UCapsuleComponent* CapsuleComponent = ActorToAttachTo->GetComponentByClass<UCapsuleComponent>();
		if (ActorToAttachTo && CapsuleComponent)
		{
			CapsuleComponent->OnComponentBeginOverlap.AddDynamic(this, &UInventoryManager::HandelIntegrationOverlap);
		}
		else
		{
			UE_LOG(LogActorComponent, Error, TEXT("no collider to bind intractions to"))
		}
	}
	
}

void UInventoryManager::HandelDeath()
{
	UE_LOG(LogActor, Warning, TEXT("handled death"))
	
	//unbind check
	if (AActor* ActorToAttachTo = GetOwner())
	{
		UCapsuleComponent* CapsuleComponent = ActorToAttachTo->GetComponentByClass<UCapsuleComponent>();
		if (ActorToAttachTo && CapsuleComponent)
		{
			CapsuleComponent->OnComponentBeginOverlap.RemoveDynamic(this, &UInventoryManager::HandelIntegrationOverlap);
		}
		else
		{
			UE_LOG(LogActorComponent, Error, TEXT("no collider to remove bind interactions from"))
		}
	}
	
	//drop all the items
	for (AItem* Item : Items)
	{
		Item->SetActorLocation(GetOwner()->GetActorLocation());
		Item->EnableItem();
	}
}


// Called when the game starts
void UInventoryManager::BeginPlay()
{
	Super::BeginPlay();

	// ...
	if (ItemToWin)
	{
		if (APlayerCharacter* Player = Cast<APlayerCharacter>(GetOwner()))
		{
			CanResaveWiningItem = true;
			Player->OnDeath.AddUObject(this, &UInventoryManager::HandelDeath);
		}
		else if (AEnemyCharacter* Enemy = Cast<AEnemyCharacter>(GetOwner()))
		{
			CanResaveWiningItem = false;
			Enemy->OnDeath.AddUObject(this, &UInventoryManager::HandelDeath);
		}
	}
}

void UInventoryManager::HandelIntegrationOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	
	//if item pick up
	if (AItem* Item = Cast<AItem>(OtherActor))
	{
		if (CanResaveWiningItem && Item->GetClass() ==  ItemToWin)
		{
			//do win stuff
			Item->DisableItem();
			Items.Add(Item);
			if (APlayerCharacter* Player = Cast<APlayerCharacter>(GetOwner()))
			{
				Player->EndGame();
			}
			return;
		}
		Item->DisableItem();
		Items.Add(Item);
		// Get the game mode
		if (AGameModeBase* GameMode = UGameplayStatics::GetGameMode(this))
		{
			// Cast it to our specific game mode
			if (AAssignment2GameMode* AssignmentGameMode = Cast<AAssignment2GameMode>(GameMode))
			{
				// Send a basic message
				FStringFormatNamedArguments Arguments;
				Arguments.Add(TEXT("ITEMNAME"), FStringFormatArg(Item->GetItemName().GetPlainNameString()));
				
				FString NewMessage = FString::Format(TEXT("I just picked up {ITEMNAME}"), Arguments);
				AssignmentGameMode->SendCharacterMessage(NewMessage);
			}
		}
	}
	//if door
	else if (AADoor* Door = Cast<AADoor>(OtherActor))
	{
		if (!Door->IsLocked)
		{
			return;
		}
		
		int ItemIndex = 0;
		bool FoundKey = false;
		int InventorySize = Items.Num();
		while (ItemIndex < InventorySize && !FoundKey)
		{
			if (AItem* ItemToTest = Items[ItemIndex])
			{
				if (Door->TryKeyToUnlock(ItemToTest))
				{
					Items.Remove(ItemToTest);
					ItemToTest->Destroy();
					FoundKey = true;
				}
			}
			ItemIndex++;
		}
	}
}


// Called every frame
void UInventoryManager::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

