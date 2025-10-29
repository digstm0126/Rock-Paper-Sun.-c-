// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Actor.h"
#include "ADoor.generated.h"

UCLASS()
class ASSIGNMENT2_API AADoor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AADoor();

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* DoorMesh;

	UPROPERTY(EditAnywhere)
	UCapsuleComponent* DetectionCapsule;
	UPROPERTY(EditAnywhere)
	float DetectionCapsuleRadius = 1;

	UPROPERTY(EditAnywhere)
	FRotator OpenRotation;
	UPROPERTY(EditAnywhere)
	FRotator ClosedRotation;
	UPROPERTY(EditAnywhere)
	float DoorSpeed;

	UPROPERTY(EditAnywhere)
	AItem* KeyForDoor = nullptr;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AItem> KeyToSpawn;

	UFUNCTION(CallInEditor)
	void LockDoor();
	
	//if KeyToTry is the right key Unlocks door and returns true. returns false if not unlocked.
	bool TryKeyToUnlock(AItem* KeyToTry);
	
	UFUNCTION(CallInEditor)
	void UnlockDoor();

	UPROPERTY(EditAnywhere)
	bool IsLocked = true;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	bool IsOpen = false;

	UFUNCTION()
	void HandelDetectionOverlap(UPrimitiveComponent* OverlappedComponent,
							AActor* OtherActor,
							UPrimitiveComponent* OtherComp,
							int32 OtherBodyIndex,
							bool bFromSweep,
							const FHitResult& SweepResult);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	

};
