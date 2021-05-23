// Copyright Jonathan Justin Rampersad 2021

#include "OpenDoor.h"

#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Actor.h"


#define OUT

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

	FindAudioComponent();
	FindPressurePlate();

	InitialYaw = OwningActorRef->GetActorRotation().Yaw;
	OpenAngle += InitialYaw;
	CurrentYaw = InitialYaw;
}

// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (PressurePlate && TotalMassOfActors() > MassToOpen)
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

	if (!DoorSound) { return; }
	if (!DoorOpened)
	{
		DoorSound->Play();
		DoorOpened = !DoorOpened;
	}
}

void UOpenDoor::CloseDoor(float& DeltaTime)
{
	CurrentYaw = FMath::FInterpTo(CurrentYaw, InitialYaw, DeltaTime, DoorCloseSpeed);
	FRotator DoorRot = OwningActorRef->GetActorRotation();

	DoorRot.Yaw = CurrentYaw;
	OwningActorRef->SetActorRotation(DoorRot);

	if (!DoorSound) { return; }
	if (DoorOpened)
	{
		DoorSound->Play();
		DoorOpened = !DoorOpened;
	}
}

float UOpenDoor::TotalMassOfActors() const
{
	if (PressurePlate == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("PressurePlate reference not made on: %s"), *GetOwner()->GetName());
		return 0.f;
	}

	float TotalMass = 0.f;

	// Find All Overlapping Actors.
	TArray<AActor*> OverlappingActors;
	PressurePlate->GetOverlappingActors(OUT OverlappingActors);

	// Add up their masses
	for (AActor* Actor: OverlappingActors)
	{
		TotalMass += Actor->FindComponentByClass<UMeshComponent>()->GetMass();
		UE_LOG(LogTemp, Warning, TEXT("%s is on the PressurePlate!"), *Actor->GetName());
	}

	return TotalMass;
}

void UOpenDoor::FindAudioComponent()
{
	DoorSound = OwningActorRef->FindComponentByClass<UAudioComponent>();
	if (!DoorSound)
	{
		UE_LOG(LogTemp, Error, TEXT("%s is Missing audio component!"), *OwningActorRef->GetName());
	}
}

void UOpenDoor::FindPressurePlate()
{
	if (PressurePlate == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("PressurePlate reference not made on: %s"), *GetOwner()->GetName());
	}
}
