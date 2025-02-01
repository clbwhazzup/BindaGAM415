// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PerlinProcTerrain.generated.h"

class UProceduralMeshComponent;
class UMaterialInterface;

UCLASS()
class BINDAFIRSTPERSON415_API APerlinProcTerrain : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APerlinProcTerrain();

	// Total size of mesh
	UPROPERTY(EditAnywhere, Meta = (ClampMin = 0))
		int XSize = 0;

	UPROPERTY(EditAnywhere, Meta = (ClampMin = 0))
		int YSize = 0;

	// Height multiplier
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ClampMin = 0))
		float ZMultiplier = 1.0f;

	// Noise zoom
	UPROPERTY(EditAnywhere, Meta = (ClampMin = 0))
		float NoiseScale = 1.0f;

	// Mesh scale to change total size, rather than adding more vertices
	UPROPERTY(EditAnywhere, Meta = (ClampMin = 0.000001))
		float Scale = 0;

	// UV scale
	UPROPERTY(EditAnywhere, Meta = (ClampMin = 0.000001))
		float UVScale = 0;

	// Radius for changing terrain in runtime
	UPROPERTY(EditAnywhere)
		float radius;

	// Depth for changing terrain in runtime
	UPROPERTY(EditAnywhere)
		FVector Depth;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Material
	UPROPERTY(EditAnywhere)
		UMaterialInterface* Mat;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Function to change mesh in runtime
	UFUNCTION()
		 void AlterMesh(FVector impactPoint);


private:
	// Procedural mesh and relevant vairable
	UProceduralMeshComponent* ProcMesh;
	TArray<FVector> Vertices;
	TArray<int> Triangles;
	TArray<FVector2D> UV0;
	TArray<FVector> Normals;
	TArray<FColor> UpVertexColors;

	// Used to keep track of sections, not used yet
	int sectionID = 0;

	// Functions for generating terrain mesh
	void CreateVertices();
	void CreateTriangles();
};
