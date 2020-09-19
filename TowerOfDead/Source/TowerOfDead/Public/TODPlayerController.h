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

	bool GetIsMove() { return isMove; }
	void SetIsMove(bool move) { isMove = move; }

private:
	void UpDown(float AxisValue);
	void LeftRight(float AxisValue);
	void Turn(float AxisValue);
	void LookUp(float AxisValue);

	void Attack();
private:
	ATODCharacter* CPlayer;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Controller, Meta = (AllowPrivateAccess = true))
	bool isMove;
};
