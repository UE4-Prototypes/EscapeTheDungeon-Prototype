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
}

// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	// Get Commonly used References
	WorldRef = GetWorld();
	PlayerControllerRef = WorldRef->GetFirstPlayerController();
	OwningActorRef = GetOwner();

	FindPhysicsHandle();
	SetupInputComponent();
}

// Checking for Physics Handle Component
void UGrabber::FindPhysicsHandle()
{
	PhysicsHandle = OwningActorRef->FindComponentByClass<UPhysicsHandleComponent>();

	if (PhysicsHandle == nullptr)
		UE_LOG(LogTemp, Error, TEXT("No physics handle component found on %s!"), *OwningActorRef->GetName());
}

void UGrabber::SetupInputComponent()
{
	InputComponent = OwningActorRef->FindComponentByClass<UInputComponent>();
	if (InputComponent)
	{
		InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);
	}
}

void UGrabber::Grab()
{
	if (PhysicsHandle == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("No physics handle component found on %s!"), *OwningActorRef->GetName());
		return;
	}

	// Only raycast when key is pressed and see if we reach any actors with physics body collision channel set.
	FHitResult HitResult = GetFirstPhysicsBodyInReach();
	UPrimitiveComponent* ComponentToGrab = HitResult.GetComponent();
	AActor* ActorHit = HitResult.GetActor();

	// If we hit something
	if (ActorHit)
		// Attach physics handle
		PhysicsHandle->GrabComponentAtLocation
		(
			ComponentToGrab,
			NAME_None,
			LineTraceEnd
		);
}

void UGrabber::Release()
{
	// Release Grabbed Object
	PhysicsHandle->ReleaseComponent();
}

// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Get player's viewpoint
	PlayerControllerRef->GetPlayerViewPoint(
		OUT PlayerViewPointLocation,
		OUT PlayerViewPointRotation);

	// Set endpoint of ray-cast
	LineTraceEnd = PlayerViewPointLocation + (PlayerViewPointRotation.Vector() * Reach);

	if (PhysicsHandle == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("No physics handle component found on %s!"), *OwningActorRef->GetName());
	}
	else
	{	
		// If the physics handle is attach
		if (PhysicsHandle->GrabbedComponent)
		{
			// Move the object we are holding
			PhysicsHandle->SetTargetLocation(LineTraceEnd);
		}
	}
}

FHitResult UGrabber::GetFirstPhysicsBodyInReach() const
{
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

	// See what ray-cast hits
	AActor* HitActor = Hit.GetActor();
	if (HitActor)
		UE_LOG(LogTemp, Warning, TEXT("%s Hit!"), *HitActor->GetName());
	
	return Hit;
}