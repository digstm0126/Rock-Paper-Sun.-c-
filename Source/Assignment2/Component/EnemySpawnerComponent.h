// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/EnemyCharacter.h"
#include "Components/ActorComponent.h"
#include "EnemySpawnerComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ASSIGNMENT2_API UEnemySpawnerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UEnemySpawnerComponent();

	UPROPERTY(EditAnywhere)
	int MaxNumEnemy = 2;

	UPROPERTY(EditAnywhere)
	int LifeTimeNumEnemy = 5;

	UPROPERTY(EditAnywhere)
	float SpawnInterval = 1;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<AEnemyCharacter> EnemyToSpawn;

	void HandelDeath();
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	AActor* ActivePlayer;

	bool IsActive = true;

	int CurrentNumEnemy = 0;

	int CurrentLifeTimeNumEnemy = 0;

	FTimerHandle SpawnTimeHandler = FTimerHandle();

	UFUNCTION()
	void HandelStartSightOverlap(UPrimitiveComponent* OverlappedComponent,
							AActor* OtherActor,
							UPrimitiveComponent* OtherComp,
							int32 OtherBodyIndex,
							bool bFromSweep,
							const FHitResult& SweepResult);

	UFUNCTION()
	void HandelEndSightOverlap(UPrimitiveComponent* OverlappedComponent,
								AActor* OtherActor,
								UPrimitiveComponent* OtherComp,
								int32 OtherBodyIndex);

	void TrySpawnEnemy();
	
public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	
};
