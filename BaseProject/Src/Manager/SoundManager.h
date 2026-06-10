#pragma once
#include <unordered_map>
#include <unordered_set>
#include <string>
#include <vector>

// 3Dサウンド用の座標構造体
struct SoundVector3 {
    float x;
    float y;
    float z;
};

// 1. まずは一番大元の識別子（enum）を定義
enum class BANK_ID {
    COMMON,
    STAGE_1,
    STAGE_2,
    STAGE_3,
    BOSS
};

enum class SOUND_ID {
    NONE,
    BGM_TITLE,
    BGM_STAGE1,
    BGM_STAGE2,
    BGM_STAGE3,
    BGM_GAMEOVER,
    BGM_GAMECLEAR,
    SE_MOVE,
    SE_DASH,
    SE_COLLISION,
    SE_JUMP,
    SE_GIMMICK_SWITCH,
    SE_GIMMICK_COLLECT,
    SE_TARAI_DROP,
    SE_ENEMY_FIRE,
	SE_CLICK,
    SE_CURSOR,
};

// 2. 音リソース構造体を定義
struct SoundResource {
    SOUND_ID id;
    std::string path;
    int handleId_ = -1;
    bool isBgm = false;

    SoundResource(SOUND_ID sId, const std::string& sPath, bool bgm = false)
        : id(sId), path(sPath), isBgm(bgm) {
    }
};

// 3. バンク構造体を定義（上の SoundResource を安全に使える）
struct SoundBank {
    BANK_ID id;
    std::string bankName;
    std::vector<SoundResource*> containingSounds;
};

class SoundManager
{
private:
    static SoundManager* instance_;

    std::unordered_map<BANK_ID, SoundBank*> bankMap_;
    std::unordered_map<SOUND_ID, SoundResource*> soundMap_;
    std::unordered_set<BANK_ID> loadedBanks_;

    SoundManager(void);
    ~SoundManager(void) {}

public:
    static void CreateInstance(void);
    static SoundManager& GetInstance(void);

    void Init(void);
    void Destroy(void);

    bool LoadBank(BANK_ID bankId);
    void UnloadBank(BANK_ID bankId);

    void PlayEvent(SOUND_ID soundId, bool loop = false);
    void Play3DEvent(SOUND_ID soundId, SoundVector3 position, float radius = 500.0f);
    void StopEvent(SOUND_ID soundId);
    bool IsPlaying(SOUND_ID soundId);
};