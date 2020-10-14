#include "TODGameMode.h"
#include "TODCharacter.h"
#include "TODPlayerController.h"
#include "TODPlayerState.h"
#include "TODUserWidget.h"
#include "TODStageManager.h"
#include "Runtime/LevelSequence/Public/LevelSequenceActor.h"

ATODGameMode::ATODGameMode()
{
	DefaultPawnClass = ATODCharacter::StaticClass();
	PlayerControllerClass = ATODPlayerController::StaticClass();
	PlayerStateClass = ATODPlayerState::StaticClass();

	static ConstructorHelpers::FClassFinder<APawn> BP_PAWN_C(TEXT("/Game/BluePrint/BP_Player.BP_Player_C"));
	if (BP_PAWN_C.Succeeded())
		DefaultPawnClass = BP_PAWN_C.Class;

	static ConstructorHelpers::FClassFinder<UTODUserWidget> BP_UI(TEXT("/Game/UI/InGame_UI.InGame_UI_C"));
	if (BP_UI.Succeeded())
		HUDWidgetClass = BP_UI.Class;

	IsSequencePlaying = false;
}

void ATODGameMode::BeginPlay()
{
	Super::BeginPlay();

	if (HUDWidgetClass != nullptr)
	{
		HUDWidget = CreateWidget<UTODUserWidget>(GetWorld(), HUDWidgetClass);
		if (HUDWidget != nullptr)
			HUDWidget->AddToViewport();
	}

	TArray<AActor*>  OutActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AActor::StaticClass(), OutActors);
	for (int i = 0; i < OutActors.Num(); i++)
	{
		if (FadeInSequence != nullptr && FadeOutSequence != nullptr)
			break;

		if (OutActors[i]->ActorHasTag("FadeIn"))
			FadeInSequence = Cast<ALevelSequenceActor>(OutActors[i]);
		else if (OutActors[i]->ActorHasTag("FadeOut"))
			FadeOutSequence = Cast<ALevelSequenceActor>(OutActors[i]);
	}
}

// 플레이어 컨트롤러의 구성을 완료하는 시점
void ATODGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	auto TODPlayerState = Cast<ATODPlayerState>(NewPlayer->PlayerState);
	if (TODPlayerState != nullptr)
		TODPlayerState->InitPlayerData();
}

bool ATODGameMode::IsLoadLevel(FName name)
{
	if (ArrLoadLevel.Num() == 0)
		return false;

	if (ArrLoadLevel.Contains(name) == true)
		return true;
	else
		return false;
}

ATODStageManager* ATODGameMode::GetStageManager(FName name)
{
	int32 num;
	if (ArrLoadLevel.Find(name, num))
		return ArrStageManager[num];
	else
		return nullptr;
}

void ATODGameMode::PlayFadeIn()
{
	if (FadeInSequence == nullptr)
		return;

	FadeInSequence->SequencePlayer->Play();
}

void ATODGameMode::PlayFadeOut()
{
	if (FadeOutSequence == nullptr)
		return;
	FadeOutSequence->SequencePlayer->Play();
}