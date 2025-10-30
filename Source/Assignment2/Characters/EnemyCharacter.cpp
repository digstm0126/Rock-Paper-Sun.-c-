// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyCharacter.h"

#include "Assignment2GameMode.h"
#include "PlayerCharacter.h"
#include "Components/SphereComponent.h"
#include "Components/CapsuleComponent.h"
#include "DrawDebugHelpers.h"
#include "KismetTraceUtils.h"
#include "Component/EnemySpawnerComponent.h"
#include "Component/UInventoryManager.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Engine/CollisionProfile.h"
#include "Engine/DamageEvents.h"
#include "GeometryCollection/GeometryCollectionParticlesData.h"
#include "Kismet/GameplayStatics.h"

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
			AttackRadiusSphere->OnComponentBeginOverlap.AddDynamic(this, &AEnemyCharacter::HandelStartAttackOverlap);	
			AttackRadiusSphere->OnComponentEndOverlap.AddDynamic(this, &AEnemyCharacter::HandelEndAttackOverlap);	
			
		}
		
		if (PlayerDetectionSphere)
		{
			PlayerDetectionSphere->SetSphereRadius(PlayerDetectionRadius);

			// Make sure it's attached to the root component
			PlayerDetectionSphere->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
			PlayerDetectionSphere->OnComponentBeginOverlap.AddDynamic(this, &AEnemyCharacter::HandelStartSightOverlap);	
			PlayerDetectionSphere->OnComponentEndOverlap.AddDynamic(this, &AEnemyCharacter::HandelEndSightOverlap);	
		}
	}

	SetUpItem();
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

bool AEnemyCharacter::IsInPersecute()
{
	if (ActivePlayer)
	{
		return true;
	}
	return false;
}

void AEnemyCharacter::HandelDeath()
{
	OnDeath.Broadcast();
	if (UInventoryManager* InventoryManager = GetOwner()->FindComponentByClass<UInventoryManager>())
	{
		InventoryManager->HandelDeath();
	}

	if (UEnemySpawnerComponent* InventoryManager = GetOwner()->FindComponentByClass<UEnemySpawnerComponent>())
	{
		InventoryManager->HandelDeath();
	}

	Ragdoll();
}

void AEnemyCharacter::SetUpItem()
{
	RightHandItem = CreateDefaultSubobject<UStaticMeshComponent>(FName("SwordMesh"));
	if (!RightHandItem)
	{
		UE_LOG(LogCore, Error, TEXT("no mesh to bind to"))
		return;
	}
	
	RightHandItem->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, FName("Hand_R"));
	RightHandItem->OnComponentBeginOverlap.AddDynamic(this, &AEnemyCharacter::HandelHit);
}

void AEnemyCharacter::HandelHit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//check if bing used is atacking
	if (!IsAttacking())
	{
		return;
	}
	
	//check if not user
	if (OtherActor == this)
	{
		return;
	}

	
	//checks if it's a Damageable Actor
	if (OtherActor->CanBeDamaged())
	{
		//check Actor is one of the hit able classes
		if (AttackableActors.Contains(OtherActor->GetClass()))
		{
			UCapsuleComponent* Capsule = Cast<UCapsuleComponent>(OtherComp);
			if (Capsule == nullptr)
			{
				return;
			}

			//sets up event
			FPointDamageEvent DamageEvent = FPointDamageEvent();
			DamageEvent.HitInfo = SweepResult;
			DamageEvent.Damage = Damage;
			DamageEvent.HitInfo.GetActor()->TakeDamage(Damage, DamageEvent, GetController(), this);
			
			StopAttacking();
		}
	}
}

bool AEnemyCharacter::CheckSightToActivePlayer()
{
	if (ActivePlayer == nullptr)
	{
		return false;
	}
	
	FHitResult HitResult;
	
	DrawDebugLineTraceSingle(GetWorld(), GetActorLocation(), ActivePlayer->GetActorLocation(),
	EDrawDebugTrace::ForOneFrame, false, HitResult, FLinearColor::Red, FLinearColor::Green,
	1);
	
	if (GetWorld()->LineTraceSingleByProfile(HitResult, GetActorLocation(), ActivePlayer->GetActorLocation(),
	FName("IgnoreOnlyPawn"), FCollisionQueryParams::DefaultQueryParam))
	{
		return false;
	}
	
	
	return true;
}

void AEnemyCharacter::HandelAttack()
{
	if (CheckSightToActivePlayer())
	{
		StartAttacking();
	}
	else
	{

		//stops attacking if attacking
		if (IsAttacking())
		{
			StopAttacking();
		}
	}
}


// Called every frame
void AEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!CheckSightToActivePlayer())
	{
		return;
	}
	
	if (!IsAttacking())
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

void AEnemyCharacter::HandelStartSightOverlap(UPrimitiveComponent* OverlappedComponent,
                                         AActor* OtherActor,
                                         UPrimitiveComponent* OtherComp,
                                         int32 OtherBodyIndex,
                                         bool bFromSweep,
                                         const FHitResult& SweepResult)
{
	if (APlayerCharacter* Player = Cast<APlayerCharacter>(OtherActor))
	{
		if (UCapsuleComponent* Capsule = Cast<UCapsuleComponent>(OtherComp))
		{
			ActivePlayer = Player;
			UE_LOG(LogActor, Log, TEXT("found new player to chase"))
		}
	}
}

void AEnemyCharacter::HandelEndSightOverlap(UPrimitiveComponent* OverlappedComponent,
											AActor* OtherActor,
											UPrimitiveComponent* OtherComp,
											int32 OtherBodyIndex)
{
	if (ActivePlayer == nullptr)
	{
		return;
	}
	 
	if (OtherActor == ActivePlayer)
	{
		if (UCapsuleComponent* Capsule = Cast<UCapsuleComponent>(OtherComp))
		{
			UE_LOG(LogActor, Log, TEXT("lost Player to chase"))
			ActivePlayer = nullptr;
		}
	}
}

void AEnemyCharacter::HandelStartAttackOverlap(UPrimitiveComponent* OverlappedComponent,
												AActor* OtherActor,
												UPrimitiveComponent* OtherComp,
												int32 OtherBodyIndex,
												bool bFromSweep,
												const FHitResult& SweepResult)
{
	if (Cast<APlayerCharacter>(OtherActor))
	{
		if (Cast<UCapsuleComponent>(OtherComp))
		{
			if (UGameInstance* GameInstance = GetGameInstance())
			{
				FTimerManager& TimerManager = GameInstance->GetTimerManager();
				FTimerDelegate TimerDelegate;
				TimerDelegate.BindUObject(this, &AEnemyCharacter::HandelAttack);
				TimerManager.SetTimer(AttackTimerHandle, TimerDelegate, AttackInterval, true);
			}
			HandelAttack();
			UE_LOG(LogActor, Log, TEXT("Can Attack Player"))
		}
	}
}

void AEnemyCharacter::HandelEndAttackOverlap(UPrimitiveComponent* OverlappedComponent,
												AActor* OtherActor,
												UPrimitiveComponent* OtherComp,
												int32 OtherBodyIndex)
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
			if (UGameInstance* GameInstance = GetGameInstance())
			{
				FTimerManager& TimerManager = GameInstance->GetTimerManager();
				TimerManager.ClearTimer(AttackTimerHandle);
			}

			//stops attacking if attacking
			if (IsAttacking())
			{
				StopAttacking();
			}
			
			UE_LOG(LogActor, Log, TEXT("lost Player to atack"))
		}
	}
}

