// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemySpawnerComponent.h"

#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"

// Sets default values for this component's properties
UEnemySpawnerComponent::UEnemySpawnerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


void UEnemySpawnerComponent::HandelDeath()
{
	IsActive = false;
}

// Called when the game starts
void UEnemySpawnerComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	if (AEnemyCharacter* ParentEnemy = Cast<AEnemyCharacter>(GetOwner()))
	{
		if (USphereComponent* PlayerDetectionSphere = ParentEnemy->PlayerDetectionSphere)
		{
			PlayerDetectionSphere->OnComponentBeginOverlap.AddDynamic(this, &UEnemySpawnerComponent::HandelStartSightOverlap);	
			PlayerDetectionSphere->OnComponentEndOverlap.AddDynamic(this, &UEnemySpawnerComponent::HandelEndSightOverlap);
		}
	}
}

void UEnemySpawnerComponent::HandelStartSightOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	if (Cast<APlayerCharacter>(OtherActor))
	{
		if (Cast<UCapsuleComponent>(OtherComp))
		{
			ActivePlayer = OtherActor;
			if (UGameInstance* GameInstance = GetOwner()->GetGameInstance())
			{
				FTimerManager& TimerManager = GameInstance->GetTimerManager();
				FTimerDelegate TimerDelegate;
				TimerDelegate.BindUObject(this, &UEnemySpawnerComponent::TrySpawnEnemy);
				TimerManager.SetTimer(SpawnTimeHandler, TimerDelegate, SpawnInterval, true);
			}
			TrySpawnEnemy();
			UE_LOG(LogActor, Log, TEXT("found player, started spawning"))
		}
	}
}

void UEnemySpawnerComponent::HandelEndSightOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (ActivePlayer == nullptr)
	{
		return;
	}
	 
	if (OtherActor == ActivePlayer)
	{
		if (Cast<UCapsuleComponent>(OtherComp))
		{
			//removes timer
			if (UGameInstance* GameInstance = GetOwner()->GetGameInstance())
			{
				FTimerManager& TimerManager = GameInstance->GetTimerManager();
				TimerManager.ClearTimer(SpawnTimeHandler);
			}
			
			ActivePlayer = nullptr;
			UE_LOG(LogActor, Log, TEXT("lost Player stopped spawning "))
		}
	}
}

void UEnemySpawnerComponent::TrySpawnEnemy()
{
	if (!IsActive)
	{
		return;
	}

	if (!ActivePlayer)
	{
		return;
	}

	if (CurrentNumEnemy >= MaxNumEnemy)
	{
		return;
	}

	if (CurrentLifeTimeNumEnemy >= LifeTimeNumEnemy)
	{
		IsActive = false;
		UE_LOG(LogActorComponent, Warning, TEXT("over live time enemy's set to inactive"))
		return;
	}

	
	if (UWorld* World = GetWorld())
	{
		FVector SpawnLocation = GetOwner()->GetActorLocation();
		FRotator SpawnRotation = GetOwner()->GetActorRotation();
		
		if (AActor* SpawnedEnemy = World->SpawnActor(EnemyToSpawn, &SpawnLocation, &SpawnRotation))
		{
			SpawnedEnemy.
			CurrentNumEnemy++;
		}
		
	}	

	

	
}


// Called every frame
void UEnemySpawnerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

