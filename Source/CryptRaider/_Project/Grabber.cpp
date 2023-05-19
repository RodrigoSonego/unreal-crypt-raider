// Fill out your copyright notice in the Description page of Project Settings.


#include "Grabber.h"

//	Trace Channel é definido em Config/DefaultEngine.ini e tem esse id bizonho aí, melhor definir
//	uma constante pra chamar ele
#define GRAB_CHANNEL ECC_GameTraceChannel2

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

	// ...
	
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction); 

	FVector StartPos = GetComponentLocation();

	FVector Distance = GetForwardVector() * MaxGrabDistance;
	FVector EndPos = StartPos + Distance;
	FVector HalfPos = StartPos + (Distance / 2);

	DrawDebugLine(GetWorld(), StartPos, EndPos, FColor::Red);
	DrawDebugSphere(GetWorld(), HalfPos, GrabRadius, 12, FColor::Red);
	
	FCollisionShape Sphere = FCollisionShape::MakeSphere(GrabRadius);

	FHitResult HitResult;
	bool HasHit = GetWorld()->SweepSingleByChannel(
		HitResult,
		StartPos,
		EndPos,
		FQuat::Identity,
		GRAB_CHANNEL,
		Sphere
	);

	if (HasHit)
	{
		FString ActorName = HitResult.GetActor()->GetActorNameOrLabel();
		UE_LOG(LogTemp, Display, TEXT("Actor Name: %s"), *ActorName);
	}
}
