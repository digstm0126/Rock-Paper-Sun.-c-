// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UObject/Class.h"
#include "Item.generated.h"

UCLASS()
class ASSIGNMENT2_API AItem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItem();

	void EnableItem();
	void DisableItem();
	
	UPROPERTY(EditAnywhere)
	class USphereComponent* InteractionSphere;
	
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* Mesh;

	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
