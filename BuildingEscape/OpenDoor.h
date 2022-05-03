// Copyright Sean Huebl 2021

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/TriggerVolume.h"
#include "OpenDoor.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDINGESCAPE_API UOpenDoor : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UOpenDoor();
	

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void OpenDoor(float DeltaTime);
	void CloseDoor(float DeltaTime);
	float TotalMassOfActors() const;
	void FindAudioComponent();
	void FindPressurePlate();

	// Tracks if sound has been played
	bool OpenDoorSound = false;
	bool CloseDoorSound = true;

private:
	UPROPERTY(EDITANYWHERE)
	float MassToOpenDoor = 50.f;
	
	UPROPERTY(EDITANYWHERE)
	float OpenAngle = 90;

	float InitialYaw;

	float CurrentYaw;
	
	float DoorLastOpened = 0.f;

	UPROPERTY(EDITANYWHERE)
	float DoorCloseDelay = 2.f;

	UPROPERTY(EDITANYWHERE)
	float DoorOpenSpeed = 1.f;

	UPROPERTY(EDITANYWHERE)
	float DoorCloseSpeed = 2.f;
	
	UPROPERTY(EDITANYWHERE)
	ATriggerVolume* PressurePlate = nullptr;

	UPROPERTY()
	UAudioComponent* AudioComponent = nullptr;
		
};
