// Fill out your copyright notice in the Description page of Project Settings.


#include "TriggerComponent.h"

UTriggerComponent::UTriggerComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UTriggerComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UTriggerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//TODO: Refactor this shenanigan into an event
	AActor* TriggerActor = GetTriggerActorOverlapping();

	if (TriggerActor != nullptr)
	{
		UPrimitiveComponent* PrimComponent = Cast<UPrimitiveComponent>(TriggerActor->GetRootComponent());
		if (PrimComponent == nullptr) { return; }

		PrimComponent->SetSimulatePhysics(false);

		AttachTriggerActor(TriggerActor);

		TriggerMovers(true);
	}
	else
	{
		TriggerMovers(false);
	}
}

AActor* UTriggerComponent::GetTriggerActorOverlapping() const
{
	TArray <AActor*> Actors;
	GetOverlappingActors(Actors);
	for (AActor* actor : Actors)
	{
		bool HasTriggerTag = actor->ActorHasTag(TagToTrigger);
		bool IsGrabbed = actor->ActorHasTag("Grabbed");

		if (HasTriggerTag && IsGrabbed == false)
		{
			return actor;
		}
	}

	return nullptr;
}

void UTriggerComponent::TriggerMovers(bool WillMove)
{
	for(AActor* Actor : MoverActors)
	{
		UMover* Mover = Actor->FindComponentByClass<UMover>();

		if (Mover == nullptr) { continue; }

		Mover->SetShouldMoveToTarget(WillMove);
	}
}

void UTriggerComponent::AttachTriggerActor(AActor* TriggerActor)
{
	FAttachmentTransformRules Rules = FAttachmentTransformRules(EAttachmentRule::KeepWorld, true);
	TriggerActor->AttachToActor(GetOwner(), Rules);
	TriggerActor->SetActorRelativeLocation(RelativeSnapPosition);
	TriggerActor->SetActorRelativeRotation(FQuat::Identity);
}
