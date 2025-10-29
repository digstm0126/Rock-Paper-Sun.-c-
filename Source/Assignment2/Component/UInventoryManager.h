// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Item.h"
#include "UInventoryManager.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ASSIGNMENT2_API UInventoryManager : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInventoryManager();

	void HandelDeath();

	UPROPERTY(VisibleAnywhere)
	TArray<AItem*> Items = TArray<AItem*>(); 

	UPROPERTY(EditAnywhere)
	TSubclassOf<AItem> ItemToWin = nullptr;
	
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	//whether this inventory can reserve a wining item and end the game
	bool CanResaveWiningItem = false;
	
	UFUNCTION()
	void HandelIntegrationOverlap(UPrimitiveComponent* OverlappedComponent,
							AActor* OtherActor,
							UPrimitiveComponent* OtherComp,
							int32 OtherBodyIndex,
							bool bFromSweep,
							const FHitResult& SweepResult);

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
