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
	// 게이지 채워서 공격?
	void HardAttack();
	void HardAttackEnd();
	// 무기 투척하기 (특정 위치에)
	void SpecialAttack();

private:
	ATODCharacter* CPlayer;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Controller, Meta = (AllowPrivateAccess = true))
	bool isMove;
};
