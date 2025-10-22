// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerCharacter.h"
#include "Component/UHealthManagerComponent.h"
#include "GameFramework/Character.h"
#include "EnemyCharacter.generated.h"

UCLASS()
class ASSIGNMENT2_API AEnemyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemyCharacter();

	// Bind to the Overlap and Endoverlap delegates of this component to run logic to go to the player
	UPROPERTY(EditAnywhere)
	class USphereComponent* PlayerDetectionSphere;

	// Bind to the Overlap and Endoverlap delegate of this component to run logic to attack the player
	UPROPERTY(EditAnywhere)
	class USphereComponent* AttackRadiusSphere;

	UPROPERTY(EditAnywhere)
	float AttackRadius = 100.f;

	UPROPERTY(EditAnywhere)
	float PlayerDetectionRadius = 500.f;

	UPROPERTY(EditAnywhere)
	UHealthManagerComponent* HealthManger;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	/**
	 * player currently changing if null ptr then not chasing
	 */
	AActor* ActivePlayer = nullptr;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/**
	* Cause a single attack animation loop to play
	*/
	UFUNCTION(BlueprintImplementableEvent)
	void StartAttacking();

	/**
	* Interrupt / stop the attack animation if it is currently playing
	*/
	UFUNCTION(BlueprintImplementableEvent)
	void StopAttacking();

	UFUNCTION()
	void HandelSightOverlap(UPrimitiveComponent* OverlappedComponent,
							AActor* OtherActor,
							UPrimitiveComponent* OtherComp,
							int32 OtherBodyIndex,
							bool bFromSweep,
							const FHitResult& SweepResult);

	/**
	* Query the Enemy to see if the attack animation is playing
	*/
	UFUNCTION(BlueprintImplementableEvent)
	bool IsAttacking();

	/**
	* Causes the Enemy mesh to ragdoll :'(
	*/
	UFUNCTION(BlueprintImplementableEvent)
	void Ragdoll();
};
