#include "GameData.h"

GameData& GameData::GetInstance()
{
    static GameData instance;
    return instance;
}

void GameData::Init(void)
{
    clearTime = 0;
}