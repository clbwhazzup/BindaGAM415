// Fill out your copyright notice in the Description page of Project Settings.


#include "BouncePad.h"
#include "BindaFirstPerson415Character.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ABouncePad::ABouncePad()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	boxComp = CreateDefaultSubobject<UBoxComponent>("Box Comp");

	RootComponent = boxComp;
}

// Called when the game starts or when spawned
void ABouncePad::BeginPlay()
{
	Super::BeginPlay();

	boxComp->OnComponentBeginOverlap.AddDynamic(this, &ABouncePad::OnOverlapBegin);
	boxComp->OnComponentEndOverlap.AddDynamic(this, &ABouncePad::OnOverlapEnd);

}

// Called every frame
void ABouncePad::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABouncePad::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ABindaFirstPerson415Character* playerChar = Cast<ABindaFirstPerson415Character>(OtherActor);
	if (playerChar)
	{
		playerChar->SetJumpVelocity(ABouncePad::BoostAmount);
		playerChar->Jump();
	}
}

void ABouncePad::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	ABindaFirstPerson415Character* playerChar = Cast<ABindaFirstPerson415Character>(OtherActor);
	if (playerChar)
	{
		playerChar->ResetJumpVelocity();
	}
}

