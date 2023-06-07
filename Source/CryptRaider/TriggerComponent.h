// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "_Project/Mover.h"
#include "TriggerComponent.generated.h"

/**
 * 
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class CRYPTRAIDER_API UTriggerComponent : public UBoxComponent
{
	GENERATED_BODY()

public:
	UTriggerComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	UPROPERTY(EditAnywhere)
	FName TagToTrigger;

	UPROPERTY(EditAnywhere)
	TArray<AActor*> MoverActors;

	UPROPERTY(EditAnywhere)
	FVector RelativeSnapPosition;
	

	AActor* GetTriggerActorOverlapping() const;
	void TriggerMovers();
	void AttachTriggerActor(AActor* TriggerActor);
};
