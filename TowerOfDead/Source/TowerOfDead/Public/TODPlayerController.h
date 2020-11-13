#pragma once

#include "TowerOfDead.h"
#include "TODCharacter.h"
#include "GameFramework/PlayerController.h"
#include "TODPlayerController.generated.h"

UCLASS()
class TOWEROFDEAD_API ATODPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ATODPlayerController();

	virtual void SetupInputComponent() override;

protected:
	virtual void BeginPlay() override;

	virtual void PostInitializeComponents() override;
	virtual void OnPossess(APawn* aPawn) override;

public:
	bool GetIsMove() { return IsMove; }

	void SetIsMove(bool move) { IsMove = move; }
	void SetMouseSpeed(float speed) { MouseSpeed = speed; }
	void SetCanInputAction(bool input) { CanInputAction = input; }
	void SetIsCanObjectInteraction(bool interaction) { IsCanObjectInteraction = interaction; }

	void EnemyKill(class ATODEnemy* KilledEnemy);

	class UTODUserWidget* GetUserHUDWidget() { return HUDWidget; }

private:
	void UpDown(float AxisValue);
	void LeftRight(float AxisValue);
	void Turn(float AxisValue);
	void LookUp(float AxisValue);

	void InputActionMouseLeft();

	void InputActionMouseRight();
	void InputActionMouseRightEnd();

	void InputActionKeyboardR();
	void InputActionKeyboardREnd();

	// Object와 상호작용(캐릭터 성장 & 회복)
	void ObjInteraction();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = UI)
	TSubclassOf<class UTODUserWidget> HUDWidgetClass;

private:
	UPROPERTY()
	class UTODUserWidget* HUDWidget;

	UPROPERTY()
	class ATODCharacter* CPlayer;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Controller, Meta = (AllowPrivateAccess = true))
	bool IsMove;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Controller, Meta = (AllowPrivateAccess = true))
	float MouseSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Controller, Meta = (AllowPrivateAccess = true))
	bool CanInputAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Controller, Meta = (AllowPrivateAccess = true))
	bool IsCanObjectInteraction;

	UPROPERTY()
	class ATODPlayerState* TODPlayerState;	
};
