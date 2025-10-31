// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AutomaticRoomCreation.h"
#include "Room.generated.h"

UCLASS()
class ASSIGNMENT2_API ARoom : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARoom();

	virtual void BeginDestroy() override;
	virtual void Destroyed() override; 

	UPROPERTY(EditAnywhere)
	FRoomConfiguration RoomConfiguration;

	void UpdateRoomConfiguration(FRoomConfiguration NewRoomConfiguration);

	void BuildRoom();

	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> FloorRoomPiece;
	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> ConerRoomPiece;
	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> WallRoomPiece;
	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> DoorHoleRoomPiece;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	TArray<AActor*> RoomPieces;

	void DestroyRoomPieces();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
