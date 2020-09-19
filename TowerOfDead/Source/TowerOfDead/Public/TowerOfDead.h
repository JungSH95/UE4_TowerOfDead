// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "EngineMinimal.h"


// 로깅을 위한 공용 매크로 지정(로그 카테고리 선언)
DECLARE_LOG_CATEGORY_EXTERN(TowerOfDead, Log, All);


// 로그를 사용한 함수의 실행 시점을 파악하기 위해 사용하는 매크로
// Verbosity : 로깅 수준 지정 (메세지(Log), 경고(Warning), 에러(Error) => 흰색, 노란색, 붉은색)
#define TODLOG_CALLINFO (FString(__FUNCTION__) + TEXT("(") + FString::FromInt(__LINE__) + TEXT(")"))
#define TODLOG_S(Verbosity) UE_LOG(TowerOfDead, Verbosity, TEXT("%s"), *TODLOG_CALLINFO)

// TODLOG_S 정보에 추가 정보를 지정해 로그 남기기 위한 매크로
#define TODLOG(Verbosity, Format, ...) UE_LOG(TowerOfDead, Verbosity, TEXT("%s %s"), *TODLOG_CALLINFO, \
  *FString::Printf(Format, ##__VA_ARGS__))

// 스크린에 로그 출력용 (AddOnScreenDebugMessage(key값, 출력시간, ...))
// 키가 -1 즉 음수일 경우 서로 덮어쓰지 않고 다른 행에 출력
// 키가 +1 즉 양수일 경우 앞의 로그를 덮어쓴다.  https://unrealengine.tistory.com/59
#define print(text) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White,text)

#define TODCHECK(Expr, ...) {if(!(Expr)) { TODLOG(Error, TEXT("ASSERTION : %s"), TEXT("'"#Expr"'")); return __VA_ARGS__;}}