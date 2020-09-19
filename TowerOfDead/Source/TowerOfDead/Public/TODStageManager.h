#pragma once

#include "TowerOfDead.h"
#include "LevelStreamerActor.h"
#include "TODEnemySpawnPoint.h"
#include "GameFramework/Actor.h"
#include "TODStageManager.generated.h"

UCLASS()
class TOWEROFDEAD_API ATODStageManager : public AActor
{
	GENERATED_BODY()
	
public:	
	ATODStageManager();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	// 스테이지 클리어 시? 스테이지 생성 시? 다음 스테이지 설정
	void SetNextStage();
	void InitEnemy();

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Stage, Meta = (AllowPrivateAccess = true))
	AActor* StartPoint;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Stage, Meta = (AllowPrivateAccess = true))
	bool IsClear;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Stage, Meta = (AllowPrivateAccess = true))
	ALevelStreamerActor* NextPortal;

	UPROPERTY(VisibleAnywhere)
	TArray<TSubclassOf<class ATODEnemy>> ArrEnemy;

	UPROPERTY()
	TArray<ATODEnemySpawnPoint*> EnemySpawnPoint;
};
