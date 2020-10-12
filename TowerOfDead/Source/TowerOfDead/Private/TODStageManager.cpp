#include "TODStageManager.h"
#include "TODEnemy.h"
#include "TODEnemyAIController.h"
#include "TODEnemyStatComponent.h"
#include "TODEnemySpawnPoint.h"
#include "TODCharacter.h"
#include "TODGameMode.h"

ATODStageManager::ATODStageManager()
{
	PrimaryActorTick.bCanEverTick = true;
	
	ConstructorHelpers::FClassFinder<ATODEnemy> ENEMYCLASS_1(TEXT("/Game/BluePrint/Enemy/BP_TESTEnemy_1.BP_TESTEnemy_1_C"));
	if (ENEMYCLASS_1.Class != NULL)
		ArrEnemyType.Add(ENEMYCLASS_1.Class);

	ConstructorHelpers::FClassFinder<ATODEnemy> ENEMYCLASS_2(TEXT("/Game/BluePrint/Enemy/BP_TESTEnemy_2.BP_TESTEnemy_2_C"));
	if (ENEMYCLASS_2.Class != NULL)
		ArrEnemyType.Add(ENEMYCLASS_2.Class);

	IsClear = false;
	IsBattleStart = false;
	EnemyDeadCount = 0;
}

void ATODStageManager::BeginPlay()
{
	Super::BeginPlay();

	ATODGameMode* gameMode = Cast<ATODGameMode>(GetWorld()->GetAuthGameMode());
	if (gameMode != nullptr)
		TODGameMode = gameMode;

	// StartPoint & Portal & EnemySawnPoint ã��
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
			else if (OutActors[i]->ActorHasTag("EnemySpawnPoint") && OutActors[i]->ActorHasTag(LevelName))
				EnemySpawnPoint.Add(Cast<ATODEnemySpawnPoint>(OutActors[i]));
		}
	}

	// ���� ����
	InitEnemy();
}

void ATODStageManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (TODGameMode != nullptr && !IsBattleStart)
	{
		// FadeOut ���� ���� �ƴ϶��
		if (!TODGameMode->GetIsSequencePlaying())
		{
			IsBattleStart = true;
			SetPlayerPosition();

			GetWorldTimerManager().SetTimer(StartTimerHandle, this,
				&ATODStageManager::StageStart, 1.0f, false);
		}
	}

	if (IsClear)
		SetNextStage();
}

void ATODStageManager::SetNextStage()
{
	if (NextPortal == nullptr)
		return;

	// ���������� ��������?
	NextPortal->SetNextLevel("");
	// ���� ���� �̵��� �� �ְ� �浹 �̺�Ʈ Ȱ��ȭ
	NextPortal->SetNextLevelEvent(true);
}

void ATODStageManager::SetPlayerPosition()
{
	// �÷��̾� ĳ���� ã�Ƽ� �ش� ��ġ�� �̵�
	ACharacter* playerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	if (playerCharacter != nullptr && StartPoint != nullptr)
	{
		playerCharacter->SetActorLocation(StartPoint->GetActorLocation());
		playerCharacter->GetController()->SetControlRotation(StartPoint->GetActorRotation());
	}
}

void ATODStageManager::InitEnemy()
{
	for (int i = 0; i < EnemySpawnPoint.Num(); i++)
	{
		if (EnemySpawnPoint[i]->GetEnemyNumber() > 0 && EnemySpawnPoint[i]->GetEnemyNumber() <= ArrEnemyType.Num())
		{
			ATODEnemy* Enemy = GetWorld()->SpawnActor<ATODEnemy>(ArrEnemyType[EnemySpawnPoint[i]->GetEnemyNumber() - 1],
				EnemySpawnPoint[i]->GetActorLocation(), FRotator::ZeroRotator);
			ArrEnemy.Add(Enemy);

			Enemy->EnemyStat->SetNewLevel(EnemySpawnPoint[i]->GetEnemyLevel());
			Enemy->OnEnemyDeadCheck.AddUObject(this, &ATODStageManager::StageClearCheck);
		}
	}

	print(FString::Printf(TEXT("Enemy count : %d"), ArrEnemy.Num()));
}

void ATODStageManager::StageStart()
{
	TODGameMode->PlayFadeIn();

	for (int i = 0; i < ArrEnemy.Num(); i++)
	{
		ATODEnemyAIController* EnemyAI = Cast<ATODEnemyAIController>(ArrEnemy[i]->GetController());
		if (EnemyAI != nullptr)
			EnemyAI->StartAI();
	}
}

void ATODStageManager::StageClearCheck()
{
	EnemyDeadCount++;

	if (EnemyDeadCount >= ArrEnemy.Num())
	{
		IsClear = true;
		NextPortal->SetPortalEffectActive();
		print(FString::Printf(TEXT("Stage Clear")));
	}
}