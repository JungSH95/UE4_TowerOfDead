// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "EngineMinimal.h"


// �α��� ���� ���� ��ũ�� ����(�α� ī�װ� ����)
DECLARE_LOG_CATEGORY_EXTERN(TowerOfDead, Log, All);


// �α׸� ����� �Լ��� ���� ������ �ľ��ϱ� ���� ����ϴ� ��ũ��
// Verbosity : �α� ���� ���� (�޼���(Log), ���(Warning), ����(Error) => ���, �����, ������)
#define TODLOG_CALLINFO (FString(__FUNCTION__) + TEXT("(") + FString::FromInt(__LINE__) + TEXT(")"))
#define TODLOG_S(Verbosity) UE_LOG(TowerOfDead, Verbosity, TEXT("%s"), *TODLOG_CALLINFO)

// TODLOG_S ������ �߰� ������ ������ �α� ����� ���� ��ũ��
#define TODLOG(Verbosity, Format, ...) UE_LOG(TowerOfDead, Verbosity, TEXT("%s %s"), *TODLOG_CALLINFO, \
  *FString::Printf(Format, ##__VA_ARGS__))

// ��ũ���� �α� ��¿� (AddOnScreenDebugMessage(key��, ��½ð�, ...))
// Ű�� -1 �� ������ ��� ���� ����� �ʰ� �ٸ� �࿡ ���
// Ű�� +1 �� ����� ��� ���� �α׸� �����.  https://unrealengine.tistory.com/59
#define print(text) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White,text)

#define TODCHECK(Expr, ...) {if(!(Expr)) { TODLOG(Error, TEXT("ASSERTION : %s"), TEXT("'"#Expr"'")); return __VA_ARGS__;}}