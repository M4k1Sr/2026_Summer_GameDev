#include <DxLib.h>
#include "SoundManager.h"
#include "../Application.h"
SoundManager* SoundManager::instance_ = nullptr;

void SoundManager::CreateInstance(void)
{
    if (instance_ == nullptr)
    {
        instance_ = new SoundManager();
    }
    instance_->Init();
}

SoundManager& SoundManager::GetInstance(void)
{
    return *instance_;
}

// コンストラクタ
SoundManager::SoundManager(void)
{
}

void SoundManager::Init(void)
{
    // パス
    static std::string PATH_SE = Application::PATH_SOUND;

    // バンクの生成
    bankMap_[BANK_ID::COMMON] = new SoundBank{ BANK_ID::COMMON, "Common" };
    bankMap_[BANK_ID::STAGE_1] = new SoundBank{ BANK_ID::STAGE_1, "Stage1" };
    bankMap_[BANK_ID::STAGE_2] = new SoundBank{ BANK_ID::STAGE_2, "Stage2" };
    bankMap_[BANK_ID::STAGE_3] = new SoundBank{ BANK_ID::STAGE_3, "Stage3" };
    bankMap_[BANK_ID::BOSS] = new SoundBank{ BANK_ID::BOSS, "Boss" };

    // リソースの生成と登録
    SoundResource* res;

    // 共通SE
    //// // --- タイトル関係 ---
    res = new SoundResource(SOUND_ID::BGM_TITLE, PATH_SE + "BGM/Title.mp3", false);
    soundMap_[SOUND_ID::BGM_TITLE] = res;
    bankMap_[BANK_ID::COMMON]->containingSounds.push_back(res);

    res = new SoundResource(SOUND_ID::SE_JUMP, PATH_SE + "SE/Player/JumpGround.mp3", false);
    soundMap_[SOUND_ID::SE_JUMP] = res;
    bankMap_[BANK_ID::COMMON]->containingSounds.push_back(res);


    // タライ落下SE
    

    // ステージ1用BGM
    res = new SoundResource(SOUND_ID::BGM_STAGE, PATH_SE + "BGM/Stage1.mp3", true);
    soundMap_[SOUND_ID::BGM_STAGE] = res;
    bankMap_[BANK_ID::STAGE_1]->containingSounds.push_back(res);

    // ステージ2用BGM
    


    // 常駐バンクのロード(プロジェクトを開いた時点で必要な音をすぐに読み込む)
    LoadBank(BANK_ID::COMMON);
}

bool SoundManager::LoadBank(BANK_ID bankId)
{
    if (loadedBanks_.contains(bankId)) return true;

    auto bPair = bankMap_.find(bankId);
    if (bPair == bankMap_.end()) return false;

    for (auto* res : bPair->second->containingSounds)
    {
        if (res->handleId_ == -1)
        {
            res->handleId_ = LoadSoundMem(res->path.c_str(), res->isBgm ? FALSE : TRUE);
        }
    }

    loadedBanks_.insert(bankId);
    return true;
}

void SoundManager::UnloadBank(BANK_ID bankId)
{
    if (loadedBanks_.count(bankId) == 0) return;

    auto bPair = bankMap_.find(bankId);
    for (auto* res : bPair->second->containingSounds)
    {
        if (res->handleId_ != -1)
        {
            DeleteSoundMem(res->handleId_);
            res->handleId_ = -1;
        }
    }
    loadedBanks_.erase(bankId);
}

void SoundManager::PlayEvent(SOUND_ID soundId, bool loop)
{
    auto sPair = soundMap_.find(soundId);
    if (sPair == soundMap_.end() || sPair->second->handleId_ == -1) return;

    ChangeVolumeSoundMem(255, sPair->second->handleId_);

    int playType = loop ? DX_PLAYTYPE_LOOP : DX_PLAYTYPE_BACK;
    PlaySoundMem(sPair->second->handleId_, playType, TRUE);
}

void SoundManager::Play3DEvent(SOUND_ID soundId, SoundVector3 position, float radius)
{
    auto sPair = soundMap_.find(soundId);
    if (sPair == soundMap_.end() || sPair->second->handleId_ == -1) return;

    int handle = sPair->second->handleId_;
    Set3DRadiusSoundMem(radius, handle);
    VECTOR dxPos = { position.x, position.y, position.z };
    Set3DPositionSoundMem(dxPos, handle);

    PlaySoundMem(handle, DX_PLAYTYPE_BACK, TRUE);
}

void SoundManager::StopEvent(SOUND_ID soundId)
{
    auto sPair = soundMap_.find(soundId);
    if (sPair == soundMap_.end() || sPair->second->handleId_ == -1) return;

    StopSoundMem(sPair->second->handleId_);
}

void SoundManager::Destroy(void)
{
    for (auto& pair : bankMap_)
    {
        UnloadBank(pair.first);
        delete pair.second;
    }
    bankMap_.clear();

    for (auto& pair : soundMap_)
    {
        delete pair.second;
    }
    soundMap_.clear();

    delete instance_;
}