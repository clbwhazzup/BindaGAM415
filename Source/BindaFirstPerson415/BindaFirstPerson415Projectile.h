// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BindaFirstPerson415Projectile.generated.h"

class USphereComponent;
class UProjectileMovementComponent;
class UNiagaraSystem;

UCLASS(config=Game)
class ABindaFirstPerson415Projectile : public AActor
{
	GENERATED_BODY()

	// Sphere collision component
	UPROPERTY(VisibleDefaultsOnly, Category=Projectile)
		USphereComponent* CollisionComp;

	// Projectile movement component
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
		UProjectileMovementComponent* ProjectileMovement;

	// Projectile mesh component
	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* ballMesh;
	
	// Matrerial for mesh
	UPROPERTY(EditAnywhere)
		UMaterial* baseMat;

	// Color for material
	UPROPERTY(EditAnywhere)
		FLinearColor randColor;

	// Dynamic material actually used for mesh
	UPROPERTY(EditAnywhere)
		UMaterialInstanceDynamic* dmiMat;

	// Material interface used for creating dynamic material
	UPROPERTY(EditAnywhere)
		UMaterialInterface* projMat;

	// Niagara system for particle effect
	UPROPERTY(EditAnywhere)
		UNiagaraSystem* colorP;


public:
	ABindaFirstPerson415Projectile();

protected:
	void BeginPlay();

public:
	/** called when projectile hits something */
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	/** Returns CollisionComp subobject **/
	USphereComponent* GetCollisionComp() const { return CollisionComp; }
	/** Returns ProjectileMovement subobject **/
	UProjectileMovementComponent* GetProjectileMovement() const { return ProjectileMovement; }
};

