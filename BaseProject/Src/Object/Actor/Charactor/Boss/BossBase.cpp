#include "../../../../Utility/AsoUtility.h"
#include "BossBase.h"

BossBase::BossBase(const BossBase::BossData& data)
	:
	CharactorBase(),
	type_(data.type),
	stateBase_(0)
{
	// 初期座標の設定
	transform_.pos = data.defaultPos;
}

BossBase::~BossBase(void)
{
}

void BossBase::Update(void)
{
	// 状態更新
	// これからの状態更新は、stateUpdate_に代入された関数が実行される
	stateUpdate_();

	// 基底クラスの更新
	CharactorBase::Update();
}

void BossBase::Draw(void)
{
	CharactorBase::Draw();
}

void BossBase::ChangeState(int state)
{

	stateBase_ = state;

	// 各状態遷移の初期処理
	stateChanges_[stateBase_]();

}