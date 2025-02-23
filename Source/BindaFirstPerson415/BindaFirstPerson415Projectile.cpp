// Copyright Epic Games, Inc. All Rights Reserved.

#include "BindaFirstPerson415Projectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Components/DecalComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "PerlinProcTerrain.h"

ABindaFirstPerson415Projectile::ABindaFirstPerson415Projectile() 
{
	// Use a sphere as a simple collision representation
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(5.0f);
	CollisionComp->BodyInstance.SetCollisionProfileName("Projectile");
	CollisionComp->OnComponentHit.AddDynamic(this, &ABindaFirstPerson415Projectile::OnHit);		// set up a notification for when this component hits something blocking

	// Players can't walk on it
	CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CollisionComp->CanCharacterStepUpOn = ECB_No;

	// Setting the mesh in
	ballMesh = CreateDefaultSubobject<UStaticMeshComponent>("Ball Mesh");

	// Set as root component
	RootComponent = CollisionComp;

	// Connects mesh to root component
	ballMesh->SetupAttachment(CollisionComp);

	// Use a ProjectileMovementComponent to govern this projectile's movement
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = CollisionComp;
	ProjectileMovement->InitialSpeed = 3000.f;
	ProjectileMovement->MaxSpeed = 3000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = true;

	// Die after 3 seconds by default
	InitialLifeSpan = 3.0f;
}

// Used to set the dynamic material and color for projectile mesh
void ABindaFirstPerson415Projectile::BeginPlay()
{
	Super::BeginPlay();

	//Gets a random color
	randColor = FLinearColor(UKismetMathLibrary::RandomFloatInRange(0.f, 1.f), UKismetMathLibrary::RandomFloatInRange(0.f, 1.f), UKismetMathLibrary::RandomFloatInRange(0.f, 1.f), 1.f);

	// Creates dynamic material instance with the material interface, and changes mesh material to the dynamic material
	dmiMat = UMaterialInstanceDynamic::Create(projMat, this);
	ballMesh->SetMaterial(0, dmiMat);

	// Sets the random color of the mesh
	dmiMat->SetVectorParameterValue("ProjColor", randColor);


	if (colorP)
	{
		UNiagaraComponent* particleComp = UNiagaraFunctionLibrary::SpawnSystemAttached(colorP, ballMesh, NAME_None, FVector(0.f), FRotator(0.f), EAttachLocation::KeepRelativeOffset, true);
		particleComp->SetNiagaraVariableLinearColor(FString("RandColor"), randColor);
	}
}


void ABindaFirstPerson415Projectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	// Only add impulse and destroy projectile if we hit a physics
	if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr) && OtherComp->IsSimulatingPhysics())
	{
		OtherComp->AddImpulseAtLocation(GetVelocity() * 100.0f, GetActorLocation());

		Destroy();
	}

	// If projectile hit something, spawns a paintball-like splat decal and particles
	if (OtherActor != nullptr) 
	{
		// Spawn particle effect, set random color, destroy mesh, and disable collision
		if (colorP)
		{
			UNiagaraComponent* particleComp = UNiagaraFunctionLibrary::SpawnSystemAttached(colorP, HitComp, NAME_None, FVector(0.f), FRotator(0.f), EAttachLocation::KeepRelativeOffset, true);
			particleComp->SetNiagaraVariableLinearColor(FString("RandColor"), randColor);

			ballMesh->DestroyComponent();
			CollisionComp->BodyInstance.SetCollisionProfileName("NoCollision");
		}

		// Random float frameNum 0-3
		float frameNum = UKismetMathLibrary::RandomFloatInRange(0.f, 3.f);

		// Spawns decal at hit location with random size
		auto Decal = UGameplayStatics::SpawnDecalAtLocation(GetWorld(), baseMat, FVector(UKismetMathLibrary::RandomFloatInRange(20.f, 40.f)), Hit.Location, Hit.Normal.Rotation(), 0.f);

		// Creates dynamic material instance for decal and saves it to change parameters in next lines
		auto MatInstance = Decal->CreateDynamicMaterialInstance();

		// Sets a random color, which was set in begin play, and the frame of the texture
		// The decal texture is a grid of 4 textures, and frameNum is which of those 4
		MatInstance->SetVectorParameterValue("Color", randColor);
		MatInstance->SetScalarParameterValue("Frame", frameNum);

		// If hit actor is a perlin proc terrain, alter the mesh
		APerlinProcTerrain* procTerrain = Cast<APerlinProcTerrain>(OtherActor);
		if (procTerrain)
		{
			procTerrain->AlterMesh(Hit.ImpactPoint);
		}
	}
}