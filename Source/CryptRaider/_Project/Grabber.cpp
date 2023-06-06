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
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction); 

	if (IsGrabbing) 
	{
		SetGrabbedLocation();
	}
}

void UGrabber::Release()
{
	if (IsGrabbing == false) 
	{
		return;
	}

	UPhysicsHandleComponent* PhysicsHandle = GetPhysicsHandle();

	AActor* GrabbedActor = PhysicsHandle->GetGrabbedComponent()->GetOwner();

	GrabbedActor->Tags.Remove("Grabbed");

	PhysicsHandle->ReleaseComponent();
	IsGrabbing = false;
}

void UGrabber::Grab()
{
	UPhysicsHandleComponent* PhysicsHandle = GetPhysicsHandle();
	if (PhysicsHandle == nullptr) { return;	}

	FHitResult HitResult;
	bool HasHit = GetGrabbableInReach(HitResult);

	if (HasHit)
	{
		UPrimitiveComponent* HitComponent = HitResult.GetComponent();

		HitResult.GetActor()->Tags.Add("Grabbed");

		HitComponent->SetSimulatePhysics(true);
		HitComponent->DetachFromParent();
		HitComponent->WakeAllRigidBodies();

		PhysicsHandle->GrabComponentAtLocationWithRotation(
			HitComponent,
			NAME_None,
			HitResult.ImpactPoint,
			GetComponentRotation()
		);

		IsGrabbing = true;
	}
}

bool UGrabber::GetGrabbableInReach(FHitResult& OutHitResult) const
{
	FVector StartPos = GetComponentLocation();
	FVector Distance = GetForwardVector() * MaxGrabDistance;
	FVector EndPos = StartPos + Distance;

	DrawDebugLine(GetWorld(), StartPos, EndPos, FColor::Red);

	FCollisionShape Sphere = FCollisionShape::MakeSphere(GrabRadius);

	bool HasHit = GetWorld()->SweepSingleByChannel(
		OutHitResult,
		StartPos,
		EndPos,
		FQuat::Identity,
		GRAB_CHANNEL,
		Sphere
	);

	return HasHit;
}

UPhysicsHandleComponent* UGrabber::GetPhysicsHandle() const
{
	return GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
}

void UGrabber::SetGrabbedLocation()
{
	UPhysicsHandleComponent* PhysicsHandle = GetPhysicsHandle();
	if (PhysicsHandle == nullptr)
	{
		return;
	}

	FVector HoldLocation = GetComponentLocation() + GetForwardVector() * HoldDistance;
	PhysicsHandle->SetTargetLocationAndRotation(HoldLocation, GetComponentRotation());
}
