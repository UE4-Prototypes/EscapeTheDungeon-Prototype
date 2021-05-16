// Copyright Jonathan Justin Rampersad 2021

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/InputComponent.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"

#include "Grabber.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ESCAPETHEDUNGEON_API UGrabber : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGrabber();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	
public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	// Common References
	UWorld* WorldRef;
	APlayerController* PlayerControllerRef;
	AActor* OwningActorRef;
	UInputComponent* InputComponent = nullptr;
	UPhysicsHandleComponent* PhysicsHandle = nullptr;

	float Reach = 100.f;

	void Grab();
	void Release();
};
