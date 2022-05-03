// Copyright Sean Huebl 2021

#include "OpenDoor.h"
#include "Components/AudioComponent.h"
#include "Components/PrimitiveComponent.h"
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
	InitialYaw = GetOwner()->GetActorRotation().Yaw;
	CurrentYaw = InitialYaw;	
	OpenAngle += InitialYaw; // same as OpenAngle = InitialYaw + Target Yaw

	FindPressurePlate();
	FindAudioComponent();
	
}

void UOpenDoor::FindAudioComponent()
{
	AudioComponent = GetOwner()->FindComponentByClass<UAudioComponent>();

	if(!AudioComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("%s is missing audio component!!"), *GetOwner()->GetName());
	}
}
void UOpenDoor::FindPressurePlate()
{
	if (!PressurePlate)
	{
		UE_LOG(LogTemp, Error, TEXT("%s Has the open door component, but no pressureplate set."), *GetOwner()->GetName());
	}
}


// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (TotalMassOfActors() > MassToOpenDoor)
	{
		OpenDoor(DeltaTime);
		DoorLastOpened = GetWorld()->GetTimeSeconds();
	}

	else
	{
		if (GetWorld()->GetTimeSeconds() - DoorLastOpened > DoorCloseDelay)
		{
			CloseDoor(DeltaTime);
		}		
	}

	/* UE_LOG(LogTemp, Warning, TEXT("%s"), *GetOwner()->GetActorRotation().ToString());
	UE_LOG(LogTemp, Warning, TEXT("Yaw is: %f"), GetOwner()->GetActorRotation().Yaw); */

	/* float CurrentYaw = GetOwner()->GetActorRotation().Yaw;

	FRotator DoorRotation = {0.f, FMath::FInterpTo(CurrentYaw, OpenAngle, DeltaTime, 2), 0.f};
	GetOwner()->SetActorRotation(DoorRotation); */

}

void UOpenDoor::OpenDoor(float DeltaTime)
{
	CurrentYaw = FMath::FInterpTo(CurrentYaw, OpenAngle, DeltaTime, DoorOpenSpeed);
	FRotator DoorRotation = GetOwner()->GetActorRotation();
	DoorRotation.Yaw = CurrentYaw;
	GetOwner()->SetActorRotation(DoorRotation);

	CloseDoorSound = false;
	if (!AudioComponent) {return;}
	if (!OpenDoorSound)
	{
		AudioComponent->Play();
		OpenDoorSound = true;
	}
	

	
}

void UOpenDoor::CloseDoor(float DeltaTime)
{
	CurrentYaw = FMath::FInterpTo(CurrentYaw, InitialYaw, DeltaTime, DoorCloseSpeed);
	FRotator DoorRotation = GetOwner()->GetActorRotation();
	DoorRotation.Yaw = CurrentYaw;
	GetOwner()->SetActorRotation(DoorRotation);

	OpenDoorSound = false;
	if (!AudioComponent) {return;}	
	if(!CloseDoorSound)
	{	
		AudioComponent->Play();
		CloseDoorSound = true;
	}
	
}

float UOpenDoor::TotalMassOfActors() const
{
	float TotalMass = 0.f;

	// find all overlapping actors
	TArray<AActor*> OverlappingActors;

	if(!PressurePlate) {return TotalMass;} // protecting the pointer
	PressurePlate->GetOverlappingActors(OUT OverlappingActors);

	// add up their mass
	for (AActor* Actor : OverlappingActors)
	{
		TotalMass += Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
		// UE_LOG(LogTemp, Warning, TEXT("%s is on the pressure plate"), *Actor->GetName());
	}	

	return TotalMass;
}

/* FRotator CurrentRotation = GetOwner()->GetActorRotation();
	FRotator OpenDoor{0.f, 90.f, 0.f};
	
	GetOwner()->SetActorRotation(OpenDoor); */