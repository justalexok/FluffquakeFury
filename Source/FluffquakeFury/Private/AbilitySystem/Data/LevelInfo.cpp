// Copyright Alex Goulder


#include "AbilitySystem/Data/LevelInfo.h"

int32 ULevelInfo::CountLevelsInWorld(int32 WorldIndex)
{
	int32 LevelCount = 0;

	for (const FFQFLevelInfo Info : LevelInformation)
	{
		if (Info.WorldIndex == WorldIndex)
		{
			LevelCount ++;
		}
	}
	return LevelCount;
}
