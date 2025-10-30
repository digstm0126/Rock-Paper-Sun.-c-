// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Component/UHealthManagerComponent.h"
#include "GameFramework/Character.h"
#include "PlayerCharacter.generated.h"

UCLASS()
class ASSIGNMENT2_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter();

protected:
	// Called when the game starts or when actor is spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	/**
	* Interrupt / stop the attack animation if it is currently playing
	*/
	UFUNCTION(BlueprintImplementableEvent)
	void StopAttacking();

	UPROPERTY(EditAnywhere)
	UHealthManagerComponent* HealthManger;

	void HandelDeath();
	/**
	* Query the player to see if the attack animation is playing
	*/
	UFUNCTION(BlueprintImplementableEvent)
	bool IsAttacking();

	/**
	* Causes the player mesh to ragdoll, the sword to drop and simulate physics and disables the movement component. RIP player :'(
	*/
	UFUNCTION(BlueprintImplementableEvent)
	void Ragdoll();
	
	void SetRightHeldItem(UStaticMeshComponent* NewMesh);

	void SetUpItem();
	void DoDamage(const FPointDamageEvent& DamageEvent);

	UPROPERTY(EditAnywhere)
	float Damage = 1.0f;
	
	UFUNCTION()
	void HandelHit(UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult);

	UPROPERTY(EditAnywhere)
	TArray<UClass*> AttackableActors;

	UPROPERTY(EditAnywhere)
	TArray<UClass*> StarringActors;

	UFUNCTION(CallInEditor)
	void EndGame();

	UPROPERTY(EditAnywhere)
	float TimeTillQuitAfterEnd = 5;
	
	FTimerHandle QuitTimeHandler = FTimerHandle();
private:
	void QuitGame();

	/**
	* Listen to this sword's overlap events to deal damage to enemies.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* RightHandItem;

};
