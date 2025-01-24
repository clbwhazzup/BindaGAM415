// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "CubeDMIMod.generated.h"

class UNiagaraSystem;

UCLASS()
class BINDAFIRSTPERSON415_API ACubeDMIMod : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACubeDMIMod();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// For overlap
	UPROPERTY(EditAnywhere)
		UBoxComponent* boxComp;

	// Cube mesh component
	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* cubeMesh;

	// Material interface used for creating dynamic material
	UPROPERTY(EditAnywhere)
		UMaterialInterface* baseMat;

	// Dynamic material instance so parameters can be changed
	UPROPERTY()
		UMaterialInstanceDynamic* dmiMat;

	// Niagara system for particle effect
	UPROPERTY(EditAnywhere)
		UNiagaraSystem* colorP;


	// Function for overlap, used to change material parameters
	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
