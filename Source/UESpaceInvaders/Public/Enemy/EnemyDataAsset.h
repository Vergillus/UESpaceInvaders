// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "EnemyDataAsset.generated.h"


class UStaticMesh;

/**
 * 
 */
UCLASS()
class UESPACEINVADERS_API UEnemyDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere)
	TObjectPtr<UStaticMesh> Mesh;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UMaterialInstance> Material;

	UPROPERTY(EditAnywhere)
	int32 ReceivedScore;

	UPROPERTY(EditAnywhere)
	int32 MinSpawnRowIndex;

	UPROPERTY(EditAnywhere)
	int32 MaxSpawnRowIndex;

	UPROPERTY(EditAnywhere, meta=(ClampMin = "0", ClampMax = "1"))
	float AttackChancePercent = 0.5f;
};
