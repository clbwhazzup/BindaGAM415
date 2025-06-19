// Fill out your copyright notice in the Description page of Project Settings.


#include "JumpBoost.h"
#include "BindaFirstPerson415Character.h"
#include "NiagaraFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AJumpBoost::AJumpBoost()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	boxComp = CreateDefaultSubobject<UBoxComponent>("Box Comp");

	RootComponent = boxComp;

}

// Called when the game starts or when spawned
void AJumpBoost::BeginPlay()
{
	Super::BeginPlay();

	boxComp->OnComponentBeginOverlap.AddDynamic(this, &AJumpBoost::OnOverlapBegin);
	boxComp->OnComponentEndOverlap.AddDynamic(this, &AJumpBoost::OnOverlapEnd);

}

// Called every frame
void AJumpBoost::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AJumpBoost::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ABindaFirstPerson415Character* playerChar = Cast<ABindaFirstPerson415Character>(OtherActor);
	if (playerChar)
	{
		playerChar->SetJumpVelocity(AJumpBoost::BoostAmount);
		if (colorP)
		{
			particleComp = UNiagaraFunctionLibrary::SpawnSystemAttached(colorP, boxComp, NAME_None, FVector(0.f), FRotator(0.f), EAttachLocation::KeepRelativeOffset, true);
		}
	}
}

void AJumpBoost::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	ABindaFirstPerson415Character* playerChar = Cast<ABindaFirstPerson415Character>(OtherActor);
	if (playerChar)
	{
		playerChar->ResetJumpVelocity();
	}
	if (particleComp)
	{
		particleComp->Deactivate();
	}
}

