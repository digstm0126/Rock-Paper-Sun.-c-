// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyCharacter.h"

#include "AudioMixerBlueprintLibrary.h"
#include "PlayerCharacter.h"
#include "Components/SphereComponent.h"
#include "Engine/DamageEvents.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Slate/SGameLayerManager.h"

// Sets default values
AEnemyCharacter::AEnemyCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PlayerDetectionSphere = CreateDefaultSubobject<USphereComponent>(FName("PlayerDetectionSphere"));
	CharacterMovementComponent = CreateDefaultSubobject<UCharacterMovementComponent>(FName("PlayerMovementComponent"));
	AttackRadiusSphere = CreateDefaultSubobject<USphereComponent>(FName("AttackRadiusSphere"));
	HealthManger = CreateDefaultSubobject<UHealthManagerComponent>(FName("UHealthManagerComponent"));

	if (RootComponent)
	{
		if (AttackRadiusSphere)
		{
			AttackRadiusSphere->SetSphereRadius(AttackRadius);

			// Make sure it's attached to the root component
			AttackRadiusSphere->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
			PlayerDetectionSphere->OnComponentBeginOverlap.AddDynamic(this, &AEnemyCharacter::HandelSightOverlap);	
			PlayerDetectionSphere->OnComponentEndOverlap.AddDynamic(this, &AEnemyCharacter::HandelSightEndOverlap);	
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

float AEnemyCharacter::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
	class AController* EventInstigator, AActor* DamageCauser)
{
	
	if (HealthManger)
	{
		HealthManger->ReduceHealth(DamageEvent);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("HealthManger is null"));
	}
	return Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
}


bool AEnemyCharacter::CheckValidChase()
{
	if (ActivePlayer == nullptr)
	{
		return false;
	}

	//check sight **to do**
	if (true)
	{
		return true;
	}
	yer
	return false;
}

// Called every frame
void AEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//check player in view
	if (CheckValidChase())
	{
		//chase player
		FVector Direction = ActivePlayer->GetActorLocation() - GetActorLocation();
		if (CharacterMovementComponent)
		{
			CharacterMovementComponent->AddInputVector(Direction);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("CharacterMovementComponent is null"));
		}
	}
	
}

// Called to bind functionality to input
void AEnemyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

	
void AEnemyCharacter::HandelSightOverlap(UPrimitiveComponent* OverlappedComponent,
                                         AActor* OtherActor,
                                         UPrimitiveComponent* OtherComp,
                                         int32 OtherBodyIndex,
                                         bool bFromSweep,
                                         const FHitResult& SweepResult)
{
	if (APlayerCharacter* Player = Cast<APlayerCharacter>(OtherActor))
	{
		ActivePlayer = Player;
		UE_LOG(LogActor, Log, TEXT("found new player to chase"))
	}
}

void AEnemyCharacter::HandelSightEndOverlap(UPrimitiveComponent* OverlappedComponent,
											AActor* OtherActor,
											UPrimitiveComponent* OtherComp,
											int32 OtherBodyIndex)
{
	if (OtherActor == ActivePlayer)
	{
		ActivePlayer = nullptr;
	}
}

