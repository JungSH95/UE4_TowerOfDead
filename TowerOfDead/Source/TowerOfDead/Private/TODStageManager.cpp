#include "TODStageManager.h"
#include "TODEnemy.h"
#include "TODEnemyStatComponent.h"
#include "TODEnemySpawnPoint.h"

ATODStageManager::ATODStageManager()
{
	PrimaryActorTick.bCanEverTick = true;
	
	ConstructorHelpers::FClassFinder<ATODEnemy> ENEMYCLASS_1(TEXT("/Game/BluePrint/Enemy/BP_TESTEnemy_1.BP_TESTEnemy_1_C"));
	if (ENEMYCLASS_1.Class != NULL)
		ArrEnemy.Add(ENEMYCLASS_1.Class);

	ConstructorHelpers::FClassFinder<ATODEnemy> ENEMYCLASS_2(TEXT("/Game/BluePrint/Enemy/BP_TESTEnemy_2.BP_TESTEnemy_2_C"));
	if (ENEMYCLASS_2.Class != NULL)
		ArrEnemy.Add(ENEMYCLASS_2.Class);

	IsClear = false;
}

void ATODStageManager::BeginPlay()
{
	Super::BeginPlay();

	// StartPoint & Portal & EnemySawnPoint 찾기
	if (StartPoint == nullptr || NextPortal == nullptr)
	{
		TArray<AActor*>  OutActors;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AActor::StaticClass(), OutActors);
		for (int i = 0; i < OutActors.Num(); i++)
		{
			if (OutActors[i]->ActorHasTag("Portal"))
				NextPortal = Cast<ALevelStreamerActor>(OutActors[i]);
			else if (OutActors[i]->ActorHasTag("StartPoint"))
				StartPoint = OutActors[i];
			else if (OutActors[i]->ActorHasTag("EnemySpawnPoint"))
				EnemySpawnPoint.Add(Cast<ATODEnemySpawnPoint>(OutActors[i]));
		}
	}
	
	// 플레이어 캐릭터 찾아서 해당 위치로 이동
	ACharacter* playerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	if (playerCharacter != nullptr && StartPoint != nullptr)
		playerCharacter->SetActorLocation(StartPoint->GetActorLocation());

	// 몬스터 생성
	InitEnemy();
}

void ATODStageManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 임시로 클리어 시킴
	IsClear = true;
	if (IsClear)
		SetNextStage();

}

void ATODStageManager::SetNextStage()
{
	if (NextPortal == nullptr)
		return;

	// 스테이지를 랜덤으로?
	NextPortal->SetNextLevel("");
	// 다음 지역 이동할 수 있게 충돌 이벤트 활성화
	NextPortal->SetNextLevelEvent(true);
}

void ATODStageManager::InitEnemy()
{
	print(FString::Printf(TEXT("Enemy Pos count : %d"), EnemySpawnPoint.Num()));

	for (int i = 0; i < EnemySpawnPoint.Num(); i++)
	{
		if (EnemySpawnPoint[i]->GetEnemyNumber() > 0 && EnemySpawnPoint[i]->GetEnemyNumber() <= ArrEnemy.Num())
		{
			ATODEnemy* Enemy = GetWorld()->SpawnActor<ATODEnemy>(ArrEnemy[EnemySpawnPoint[i]->GetEnemyNumber() - 1],
				EnemySpawnPoint[i]->GetActorLocation(), FRotator::ZeroRotator);

			Enemy->EnemyStat->SetNewLevel(EnemySpawnPoint[i]->GetEnemyLevel());
		}
	}
}