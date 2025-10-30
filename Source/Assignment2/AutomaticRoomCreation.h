// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/PlayerCharacter.h"
#include "GameFramework/Actor.h"
#include "AutomaticRoomCreation.generated.h"


USTRUCT()
struct FRoomConfiguration
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere)
	int RoomPosX = 0;
	UPROPERTY(EditAnywhere)
	int RoomPosY = 0;
	UPROPERTY(EditAnywhere)
	int RoomSizeX = 3;
	UPROPERTY(EditAnywhere)
	int RoomSizeY = 3;

	UPROPERTY(EditAnywhere)
	float CellSize = 500;

	//room type
	
	//array of doors 
	//TArray<>
	
};

UCLASS()
class ASSIGNMENT2_API AAutomaticRoomCreation : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAutomaticRoomCreation();

	// TArray of URoomConfigurations
	UPROPERTY(EditAnywhere)
	TArray<FRoomConfiguration> Rooms;

	UPROPERTY(EditAnywhere)
	TSubclassOf<APlayerCharacter> PlayerToSpawn;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> RoomToSpawn;

	UPROPERTY(EditAnywhere)
	FVector StartLocation = FVector(0,0,0);

	UPROPERTY(EditAnywhere)
	FRotator StartRotation = FRotator(0,0,0);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	TArray<AActor*> SpawnedRooms;

	AActor* SpawnedPlayer;
	
	UFUNCTION(CallInEditor)
	void BuildRooms();
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
