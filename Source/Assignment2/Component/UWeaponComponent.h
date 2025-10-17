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

	//reference to weapon mesh
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* WeaponMesh;

	UPROPERTY(EditAnywhere)
	AActor* User;

	
	void SetUser(AActor* NewUser);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;


	void BindToWeapon();

	//on overlap
	void HandelHit();

	//deals damage to input Actor 
	void DealDamage(AActor* ActorToDamage);

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
