// Copyright Jonathan Justin Rampersad 2021


#include "OpenDoor.h"
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

	InitialYaw = GetOwner()->GetActorRotation().Yaw;
	TargetYaw += InitialYaw;
	CurrentYaw = InitialYaw;

	if (!PressurePlate)
	{
		UE_LOG(LogTemp, Error, TEXT("PressurePlate reference not made on: %s"), *GetOwner()->GetName());
	}
}


// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (PressurePlate && PressurePlate->IsOverlappingActor(ActorThatOpens))
	{
		OpenDoor(DeltaTime);
	}
}

void UOpenDoor::OpenDoor(float& DeltaTime)
{
	CurrentYaw = FMath::FInterpTo(CurrentYaw, TargetYaw, DeltaTime, 2);
	FRotator DoorRot = GetOwner()->GetActorRotation();

	DoorRot.Yaw = CurrentYaw;
	GetOwner()->SetActorRotation(DoorRot);

	// FRotator NewRotation = { 0.f, 90.f, 0.f };
	// GetOwner()->SetActorRotation(NewRotation);
}