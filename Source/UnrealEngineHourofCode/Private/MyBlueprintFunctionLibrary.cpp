// Fill out your copyright notice in the Description page of Project Settings.


#include "MyBlueprintFunctionLibrary.h"

FString UMyBlueprintFunctionLibrary::helloUnreal(int a, int b)
{
	int result = a + b;
	return FString::Printf(TEXT("Testing: The calculation %d + %d is %d"), a, b, result);
}

FString UMyBlueprintFunctionLibrary::megaNasty(FString x)
{
	return FString(x);
}
