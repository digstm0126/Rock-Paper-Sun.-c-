// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
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
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	bool IsOpen = false;
	bool IsLocked = false;

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
