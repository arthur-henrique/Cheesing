// Fill out your copyright notice in the Description page of Project Settings.


#include "CheesingGameInstance.h"

int UCheesingGameInstance::GetIdFromIndex(int index)
{
	ACollectible* item;
	collectiblesArray.Find(item, index);

	return item->id;
}

bool UCheesingGameInstance::GetCollectStateFromIndex(int index)
{
	ACollectible* item;
	collectiblesArray.Find(item, index);

	return item -> wasCollected;
}
