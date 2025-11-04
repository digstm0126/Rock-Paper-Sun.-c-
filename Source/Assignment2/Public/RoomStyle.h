// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RoomStyle.generated.h"

UCLASS()
class ASSIGNMENT2_API ARoomStyle : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARoomStyle();

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

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
