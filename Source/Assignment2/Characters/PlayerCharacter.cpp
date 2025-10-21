// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"

#include "Assignment2GameMode.h"
#include "EnemyCharacter.h"
#include "NiagaraFunctionLibrary.h"
#include "Components/CapsuleComponent.h"
#include "Engine/DamageEvents.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	
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


void APlayerCharacter::DoDamage(AEnemyCharacter* EnemyCharacter)
{
	FDamageEvent DamageEvent;
	EnemyCharacter->TakeDamage(Damage, DamageEvent, GetController(), this);
	UNiagaraFunctionLibrary::SpawnSystemAttached(Blood, RightHandItem, NAME_None, FVector3d(0,0,0), FRotator::ZeroRotator, EAttachLocation::SnapToTarget, true);

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
			UE_LOG(LogCore, Error, TEXT("can atack"));
			AEnemyCharacter* EnemyCharacter = CastChecked<AEnemyCharacter>(OtherActor);
			if (EnemyCharacter)
			{
				UNiagaraFunctionLibrary::SpawnSystemAttached(Blood, Capsule, NAME_None, FVector3d(0,0,100), FRotator::ZeroRotator, EAttachLocation::SnapToTarget, true);
				DoDamage(EnemyCharacter);
				StopAttacking();
			}
		}
	}
	
	//check if stagarable thing
	
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

void APlayerCharacter::SetRightHeldItem(UStaticMeshComponent* NewMesh)
{

	NewMesh->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, FName("Hand_R"));
	RightHandItem = NewMesh;
	
}
	


