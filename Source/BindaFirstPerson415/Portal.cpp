// Fill out your copyright notice in the Description page of Project Settings.


#include "Portal.h"
#include "BindaFirstPerson415Character.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
APortal::APortal()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	// Component setup
	mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	boxComp = CreateDefaultSubobject<UBoxComponent>("Box Comp");
	sceneCapture = CreateDefaultSubobject<USceneCaptureComponent2D>("Capture");
	rootArrow = CreateDefaultSubobject<UArrowComponent>("Root Arrow");

	RootComponent = boxComp;
	mesh->SetupAttachment(boxComp);
	sceneCapture->SetupAttachment(mesh);
	rootArrow->SetupAttachment(RootComponent);

	// Mesh ignores all collisions
	mesh->SetCollisionResponseToAllChannels(ECR_Ignore);
}

// Called when the game starts or when spawned
void APortal::BeginPlay()
{
	Super::BeginPlay();
	
	// Binds overlap event to OnOverlapBegin function
	boxComp->OnComponentBeginOverlap.AddDynamic(this, &APortal::OnOverlapBegin);

	// Hides mesh in scene capture
	mesh->SetHiddenInSceneCapture(true);

	// If material interface exists, set the material
	if (mat)
	{
		mesh->SetMaterial(0, mat);
	}

}

// Called every frame
void APortal::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	UpdatePortals();
}

// Overlap event
void APortal::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ABindaFirstPerson415Character* playerChar = Cast<ABindaFirstPerson415Character>(OtherActor);
	if (playerChar)
	{
		if(otherPortal)
		{
			if (!playerChar->isTeleporting)
			{
				// Gets location of other portal's root arrow, and teleports player. Sets isTeleporting to true for 1 second so you can't teleport back instantly
				playerChar->isTeleporting = true;
				FVector loc = otherPortal->rootArrow->GetComponentLocation();
				playerChar->SetActorLocation(loc);

				FTimerHandle TimerHandle;
				FTimerDelegate TimerDelegate;
				TimerDelegate.BindUFunction(this, "SetBool", playerChar);
				GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDelegate, 1, false);
			}
		}
	}
}

// Sets isTeleporting to false
void APortal::SetBool(ABindaFirstPerson415Character* playerChar)
{
	if (playerChar)
	{
		playerChar->isTeleporting = false;
	}
}

// Updates the location and rotation of the scene capture component so it looks like player is looking through the portal
void APortal::UpdatePortals()
{
	FVector Location = this->GetActorLocation() - otherPortal->GetActorLocation();
	FVector camLocation = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0)->GetTransformComponent()->GetComponentLocation();
	FRotator camRotation = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0)->GetTransformComponent()->GetComponentRotation();
	FVector combinedLocation = camLocation + Location;
	sceneCapture->SetWorldLocationAndRotation(combinedLocation, camRotation);
}

