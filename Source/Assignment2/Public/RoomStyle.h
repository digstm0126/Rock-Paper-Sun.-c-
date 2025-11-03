// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class ASSIGNMENT2_API RoomStyle
{
public:
	RoomStyle();
	~RoomStyle();

	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> FloorRoomPiece;
	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> ConerRoomPiece;
	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> WallRoomPiece;
	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> DoorHoleRoomPiece;
};
