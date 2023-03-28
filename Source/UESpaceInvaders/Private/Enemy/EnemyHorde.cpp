// Fill out your copyright notice in the Description page of Project Settings.


#include "UESpaceInvaders/Public/Enemy/EnemyHorde.h"

// Sets default values
AEnemyHorde::AEnemyHorde()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AEnemyHorde::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEnemyHorde::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

