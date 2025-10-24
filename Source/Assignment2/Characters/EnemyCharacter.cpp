// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyCharacter.h"

#include "PlayerCharacter.h"
#include "Components/SphereComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Engine/CollisionProfile.h"
#include "GeometryCollection/GeometryCollectionParticlesData.h"

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

	return false;
}

bool AEnemyCharacter::CheckSightToActivePlayer()
{
	if (ActivePlayer == nullptr)
	{
		return false;
	}

	
	FHitResult HitResult;
	if (GetWorld()->LineTraceSingleByProfile(HitResult, GetActorLocation(), ActivePlayer->GetActorLocation(), FName("Enemy"), FCollisionQueryParams::DefaultQueryParam))
	{
		
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
	}
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
			UE_LOG(LogActor, Log, TEXT("Can Attack Player"))
		}
	}
}

void AEnemyCharacter::HandelEndAttackOverlap(UPrimitiveComponent* OverlappedComponent,
												AActor* OtherActor,
												UPrimitiveComponent* OtherComp,
												int32 OtherBodyIndex)
{
	if (ActivePlayer == nullptr || LoopingAttack == false)
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

