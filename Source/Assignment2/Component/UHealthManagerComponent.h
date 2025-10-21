// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "UHealthManagerComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ASSIGNMENT2_API UHealthManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UHealthManagerComponent();

	UPROPERTY(EditAnywhere)
	float Health = 1;
	UPROPERTY(EditAnywhere)
	float MaxHealth = 100;
	UPROPERTY(EditAnywhere)
	bool IsAlive = true;

	//reduces health by amount to reduce. >= 0 triggers handel death
	void ReduceHealth(float AmountToReduce);
	
	void AddHealth(float AmountToAdd);
	
	void SetHealth(float NewHealth);
	

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	virtual void HandelDeath();
public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
