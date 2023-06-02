// Fill out your copyright notice in the Description page of Project Settings.


#include "TriggerComponent.h"

UTriggerComponent::UTriggerComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UTriggerComponent::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp, Display, TEXT("Trigger started"));
}

void UTriggerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	AActor* TriggerActor = GetTriggerActorOverlapping();

	if (TriggerActor != nullptr) 
	{
		TriggerMovers();
	}

}

AActor* UTriggerComponent::GetTriggerActorOverlapping() const
{
	TArray <AActor*> Actors;
	GetOverlappingActors(Actors);
	for (AActor* actor : Actors)
	{
		if (actor->ActorHasTag(TagToTrigger) == false) { continue; }

		return actor;
	}

	return nullptr;
}

void UTriggerComponent::TriggerMovers()
{
	for(AActor* Actor : MoverActors)
	{
		UMover* Mover = Actor->FindComponentByClass<UMover>();

		if (Mover == nullptr) { continue; }

		Mover->SetShouldMove(true);
	}
}
