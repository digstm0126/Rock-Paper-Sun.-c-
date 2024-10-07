// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyCharacter.h"
#include "Components/SphereComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
AEnemyCharacter::AEnemyCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PlayerDetectionSphere = CreateDefaultSubobject<USphereComponent>(FName("PlayerDetectionSphere"));
	AttackRadiusSphere = CreateDefaultSubobject<USphereComponent>(FName("AttackRadiusSphere"));

	if (RootComponent)
	{
		if (AttackRadiusSphere)
		{
			AttackRadiusSphere->SetSphereRadius(AttackRadius);

			// Make sure it's attached to the root component
			AttackRadiusSphere->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
		}
		
		if (PlayerDetectionSphere)
		{
			PlayerDetectionSphere->SetSphereRadius(PlayerDetectionRadius);

			// Make sure it's attached to the root component
			PlayerDetectionSphere->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
		}
	}
}

// Called when the game starts or when spawned
void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AEnemyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

