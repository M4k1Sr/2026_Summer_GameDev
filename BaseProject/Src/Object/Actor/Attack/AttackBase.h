#pragma once
#include <functional>
#include <map>
#include <DxLib.h>
#include "../ActorBase.h"
class AttackManager;

class AttackBase : public ActorBase
{
public:

	// 種別
	enum class TYPE
	{
		FIRE_BALL,
		WAVE_ATTACK,
		ARROW_ATTACK,
		CHARGE_ATTACK,
		AXE_THROW_ATTACK,
		MAX,
	};

	// オブジェクトデータ
	struct ObjectData
	{
		int id;	// 識別用ID
		AttackBase::TYPE type;	// 種別
		float speed;	// 移動速度
		float maxDistance;	// 最大移動距離(消滅しない距離)
		float scale;	// 描画倍率
	};

	// コンストラクタ
	AttackBase(const AttackBase::ObjectData& data);

	// デストラクタ
	~AttackBase(void) override;

	// 更新
	void Update(void) override;

	// 描画
	void Draw(void) override;

	// 解放
	void Release(void) override;

private:

	// 状態管理
	int stateBase_;

	// 状態管理(状態遷移時初期処理)
	std::map<int, std::function<void(void)>> stateChanges_;

	// 状態管理(更新ステップ)
	std::function<void(void)> stateUpdate_;

	// 状態遷移
	void ChangeState(int state);

	// 種別
	TYPE type_;

	// 攻撃管理フラグ
	bool isAttack_;

};
