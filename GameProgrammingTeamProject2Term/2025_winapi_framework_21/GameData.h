#pragma once
class GameData
{
public:
    bool boss1Cleared = false;
    bool boss2Cleared = false;
    bool boss3Cleared = false;

    float bgmVolume = 1.f;
    float effectVolume = 1.f;

    static GameData* GetInstance()
    {
        static GameData instance;
        return &instance;
    }

    void Clear()
    {
        boss1Cleared = false;
        boss2Cleared = false;
        boss3Cleared = false;
    }

    GameData(const GameData&) = delete;
    void operator=(const GameData&) = delete;

private:
    GameData() {}
};