// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"

#include "Assignment2GameMode.h"
#include "EnemyCharacter.h"
#include "NiagaraFunctionLibrary.h"
#include "Component/EnemySpawnerComponent.h"
#include "Component/UInventoryManager.h"
#include "Components/CapsuleComponent.h"
#include "Engine/DamageEvents.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	HealthManger = CreateDefaultSubobject<UHealthManagerComponent>(FName("UHealthManagerComponent"));
	
	// set up item
	SetUpItem();
}

void APlayerCharacter::SetUpItem()
{
	RightHandItem = CreateDefaultSubobject<UStaticMeshComponent>(FName("SwordMesh"));
	if (!RightHandItem)
	{
		UE_LOG(LogCore, Error, TEXT("no mesh to bind to"))
		return;
	}
	
	SetRightHeldItem(RightHandItem);
	RightHandItem->OnComponentBeginOverlap.AddDynamic(this, &APlayerCharacter::HandelHit);
	
}


void APlayerCharacter::DoDamage(const FPointDamageEvent& DamageEvent)
{
	//deals damage to actor
	DamageEvent.HitInfo.GetActor()->TakeDamage(Damage, DamageEvent, GetController(), this);
	
	//displays mesage
	// Get the game mode
	if (AGameModeBase* GameMode = UGameplayStatics::GetGameMode(this))
	{
		// Cast it to our specific game mode
		if (AAssignment2GameMode* AssignmentGameMode = Cast<AAssignment2GameMode>(GameMode))
		{
			// Send a basic message
			FStringFormatNamedArguments arg;
			arg.Add(TEXT("DAMAGE"), Damage);
			FString NewMessage( FString::Format(TEXT("Yes! I did {DAMAGE}"), arg));
			AssignmentGameMode->SendCharacterMessage(NewMessage);
		}
	}
	
}

void APlayerCharacter::HandelHit(UPrimitiveComponent* OverlappedComponent,
                                 AActor* OtherActor,
                                 UPrimitiveComponent* OtherComp,
                                 int32 OtherBodyIndex,
                                 bool bFromSweep,
                                 const FHitResult& SweepResult)
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
			DoDamage(DamageEvent);
			
			StopAttacking();
			return;
		}
	}
	
	//check if stagarable thing
	if (StarringActors.Contains(OtherActor->GetClass()))
	{
		StopAttacking();
	}
	
}

void APlayerCharacter::EndGame()
{
	if (AGameModeBase* GameMode = UGameplayStatics::GetGameMode(this))
	{
		// Cast it to our specific game mode
		if (AAssignment2GameMode* AssignmentGameMode = Cast<AAssignment2GameMode>(GameMode))
		{
			// Send a basic message
			FStringFormatNamedArguments Arguments;
				
			FString NewMessage = FString::Format(TEXT("Game Ending soon"), Arguments);
			AssignmentGameMode->SendCharacterMessage(NewMessage);
		}
	}

	if (UGameInstance* GameInstance = GetGameInstance())
	{
		FTimerManager& TimerManager = GameInstance->GetTimerManager();
		FTimerDelegate TimerDelegate;
		TimerDelegate.BindUObject(this, &APlayerCharacter::QuitGame);
		TimerManager.SetTimer(QuitTimeHandler, TimerDelegate, TimeTillQuitAfterEnd, false);
	}
}

void APlayerCharacter::QuitGame()
{
	UKismetSystemLibrary::QuitGame(GetWorld(), GetLocalViewingPlayerController(), EQuitPreference::Quit, false);
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

float APlayerCharacter::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
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

void APlayerCharacter::HandelDeath()
{
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

void APlayerCharacter::SetRightHeldItem(UStaticMeshComponent* NewMesh)
{

	NewMesh->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, FName("Hand_R"));
	RightHandItem = NewMesh;
	
}
	


