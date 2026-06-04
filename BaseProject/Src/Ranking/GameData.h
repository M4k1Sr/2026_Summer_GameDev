#pragma once

class GameData
{
public:
    static GameData& GetInstance();
    
	void Init(void);

    int clearTime = 0;
};