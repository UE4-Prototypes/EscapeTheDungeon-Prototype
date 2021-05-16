// Copyright Jonathan Justin Rampersad 2021

#include "Grabber.h"

#include "DrawDebugHelpers.h"

#define OUT

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	// Get Commonly used References
	WorldRef = GetWorld();
	PlayerControllerRef = WorldRef->GetFirstPlayerController();
	OwningActorRef = GetOwner();

	// Checking for Physics Handle Component
	PhysicsHandle = OwningActorRef->FindComponentByClass<UPhysicsHandleComponent>();

	if (PhysicsHandle)
	{
		// Physics handle found.
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("No physics handle component found on %s!"), *OwningActorRef->GetName())
	}

	InputComponent = OwningActorRef->FindComponentByClass<UInputComponent>();
	if (InputComponent)
	{
		InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);
	}
	// else
	// {
	// 	UE_LOG(LogTemp, Error, TEXT("Input component MISSING on %s"), *OwningActorRef->GetName());
	// }
}

void UGrabber::Grab()
{
	UE_LOG(LogTemp, Warning, TEXT("Grabber Pressed!"));
}

void UGrabber::Release()
{
	UE_LOG(LogTemp, Warning, TEXT("Grabber Released!"));
}

// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	// Get player's viewpoint
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;

	PlayerControllerRef->GetPlayerViewPoint(
		OUT PlayerViewPointLocation,
		OUT PlayerViewPointRotation);

	// Draw a line from player showing the reach
	FVector LineTraceEnd = PlayerViewPointLocation + (PlayerViewPointRotation.Vector() * Reach);

	DrawDebugLine(
		WorldRef,
		PlayerViewPointLocation,
		LineTraceEnd,
		FColor(125, 27, 245),
		false,
		0.f,
		0,
		5.f
	);

	// Ray-cast out to a certain distance (Reach)
	FHitResult Hit;
	FCollisionQueryParams TraceParams(FName(TEXT("")), false, OwningActorRef);

	WorldRef->LineTraceSingleByObjectType(
	OUT Hit,
		PlayerViewPointLocation,
		LineTraceEnd,
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParams
	);

	AActor *HitActor = Hit.GetActor();
	if(HitActor)
		UE_LOG(LogTemp, Warning, TEXT("%s Hit!"), *HitActor->GetName());
	// Logging out to test

	// See what it hits
}

