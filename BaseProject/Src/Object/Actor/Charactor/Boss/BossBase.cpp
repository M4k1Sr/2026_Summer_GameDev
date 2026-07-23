#include "BossBase.h"
#include"../../Attack/StrategyAttack.h"
#include "../../../../Utility/AsoUtility.h"
#include "../../../../Utility/MatrixUtility.h"
#include "../Player.h"
#include "../../../../Manager/SoundManager.h"

BossBase::BossBase(const BossBase::BossData& data)
	:
	CharactorBase(),
	data_(data),
	type_(data.type),
	stateBase_(0),
	player_()
{
	// 初期座標の設定
	transform_.pos = data.defaultPos;
	
	//// サウンド読み込み
	//SoundManager::GetInstance().LoadBank(BANK_ID::BOSS);

}

BossBase::~BossBase(void)
{
	//// サウンド停止
	//SoundManager::GetInstance().StopEvent(SOUND_ID::SE_ENEMY_FIRE);
}

void BossBase::SetPlayer(Player* player)
{
	player_ = player;
}

void BossBase::ChangeState(int state)
{

	stateBase_ = state;

	// 各状態遷移の初期処理
	stateChanges_[stateBase_]();

}

// BossBase.cpp の一番下（ChangeState の下など）に追記
void BossBase::ChangeAttackStrategy(std::unique_ptr<StrategyAttack> newStrategy)
{
	// ここなら StrategyAttack の中身が完全に見えているので、安全に移動・消去ができます！
	currentAttack_ = std::move(newStrategy);
}
