// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "UWeaponComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ASSIGNMENT2_API UUWeaponComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UUWeaponComponent();

	UPROPERTY(EditAnywhere)
	float WeaponsDamage = 5.0f;
	
 
	UPROPERTY(EditAnywhere)
	AActor* User;

	
	TArray<UClass*> AttackableActors;
	
	bool InUses = false;
	
	void SetUser(AActor* NewUser);

	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;


	void BindToWeapon();

	//handles when current mesh hits something
	UFUNCTION()
	void HandelHit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	//deals damage to input Actor 
	void DealDamage(AActor* ActorToDamage);

	//mesh of the weapon
	UStaticMeshComponent* WeaponMesh;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
