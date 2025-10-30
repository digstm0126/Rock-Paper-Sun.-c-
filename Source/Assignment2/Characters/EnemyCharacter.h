// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerCharacter.h"
#include "Component/UHealthManagerComponent.h"
#include "GameFramework/Character.h"
#include "UObject/Class.h"
#include "EnemyCharacter.generated.h"

UENUM()
enum class EEngagementBehaviour : uint8
{
	Pursue UMETA(DisplayName = "Pursue"),
	None UMETA(DisplayName = "None")
};

UENUM()
enum class EAttackingBehaviour : uint8
{
	Retreat UMETA(DisplayName = "Retreat"),
	Attack UMETA(DisplayName = "Attack")
};


UCLASS()
class ASSIGNMENT2_API AEnemyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemyCharacter();

	DECLARE_MULTICAST_DELEGATE(FHandelDeath)
	
	FHandelDeath OnDeath;

	UPROPERTY(EditAnywhere)
	EEngagementBehaviour EngagementBehaviour = EEngagementBehaviour::Pursue;

	UPROPERTY(EditAnywhere)
	EAttackingBehaviour AttackingBehaviour = EAttackingBehaviour::Attack;
	
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

	UPROPERTY(EditAnywhere)
	float AttackInterval = 1.6f;

	UPROPERTY(EditAnywhere)
	TArray<UClass*> AttackableActors;

	UPROPERTY(EditAnywhere)
	float Damage = 10;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	bool IsInPersecute();

	void HandelDeath();
	
protected:
	
	void SetUpItem();
	
	UFUNCTION()
	void HandelHit(UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult);

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UCharacterMovementComponent* CharacterMovementComponent = nullptr;

	/**
	 * player currently changing if null ptr then not chasing
	 */
	AActor* ActivePlayer = nullptr;
	
	FTimerHandle AttackTimerHandle = FTimerHandle();

	bool CheckValidChase();

	bool CheckSightToActivePlayer();
	
	void HandelAttack();
	void ChasePlayer();

	bool LoopingAttack;

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


	UFUNCTION()
	void HandelStartAttackOverlap(UPrimitiveComponent* OverlappedComponent,
							AActor* OtherActor,
							UPrimitiveComponent* OtherComp,
							int32 OtherBodyIndex,
							bool bFromSweep,
							const FHitResult& SweepResult);

	UFUNCTION()
	void HandelEndAttackOverlap(UPrimitiveComponent* OverlappedComponent,
								AActor* OtherActor,
								UPrimitiveComponent* OtherComp,
								int32 OtherBodyIndex);

	/**
	* Listen to this sword's overlap events to deal damage to enemies.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* RightHandItem;

	void DoAttackBehaviour();
	void EndAttackBehaviour();

	void DoEnageBehaviour();
	void EndEnageBehaviour();
	
	void RetreatFromPlayer();

	bool IsRetreating = false;
	bool IsChasing = false;

	
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
