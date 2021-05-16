// Copyright Jonathan Justin Rampersad 2021

#include "OpenDoor.h"

#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Actor.h"

// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();

	WorldRef = GetWorld();
	OwningActorRef = GetOwner();

	InitialYaw = OwningActorRef->GetActorRotation().Yaw;
	OpenAngle += InitialYaw;
	CurrentYaw = InitialYaw;

	if (!PressurePlate)
	{
		UE_LOG(LogTemp, Error, TEXT("PressurePlate reference not made on: %s"), *GetOwner()->GetName());
	}

	PlayerActor = WorldRef->GetFirstPlayerController()->GetPawn();
}


// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (PressurePlate && (PressurePlate->IsOverlappingActor(ActorThatOpens) || PressurePlate->IsOverlappingActor(PlayerActor)))
	{
		OpenDoor(DeltaTime);
		DoorLastOpened = WorldRef->GetTimeSeconds();
	}
	else
	{
		if (WorldRef->GetTimeSeconds() - DoorLastOpened > DoorCloseDelay)
		{
			CloseDoor(DeltaTime);
		}
	}
}

void UOpenDoor::OpenDoor(float& DeltaTime)
{
	CurrentYaw = FMath::FInterpTo(CurrentYaw, OpenAngle, DeltaTime, DoorOpenSpeed);
	FRotator DoorRot = OwningActorRef->GetActorRotation();

	DoorRot.Yaw = CurrentYaw;
	OwningActorRef->SetActorRotation(DoorRot);
}

void UOpenDoor::CloseDoor(float& DeltaTime)
{
	CurrentYaw = FMath::FInterpTo(CurrentYaw, InitialYaw, DeltaTime, DoorCloseSpeed);
	FRotator DoorRot = OwningActorRef->GetActorRotation();

	DoorRot.Yaw = CurrentYaw;
	OwningActorRef->SetActorRotation(DoorRot);
}