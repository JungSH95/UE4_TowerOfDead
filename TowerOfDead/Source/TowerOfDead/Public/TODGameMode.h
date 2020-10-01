#pragma once

#include "TowerOfDead.h"
#include "GameFramework/GameModeBase.h"
#include "TODGameMode.generated.h"

UCLASS()
class TOWEROFDEAD_API ATODGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	ATODGameMode();

	virtual void BeginPlay() override;
	virtual void PostLogin(APlayerController* NewPlayer) override;

	class UTODUserWidget* GetUserHUDWidget() { return HUDWidget; }

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = UI)
	TSubclassOf<class UTODUserWidget> HUDWidgetClass;

private:
	UPROPERTY()
	class UTODUserWidget* HUDWidget;
};
