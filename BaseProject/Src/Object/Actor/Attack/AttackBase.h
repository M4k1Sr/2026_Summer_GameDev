#pragma once
#include <functional>
#include <map>
#include <DxLib.h>
#include "../ActorBase.h"
class AttackManager;
class ObjectManager;
class BossData;

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
	struct AttackParam
	{
		int id;	// 識別用ID
		AttackBase::TYPE type;	// 種別
		float speed;	// 移動速度
		float maxDistance;	// 最大移動距離(消滅しない距離)
		float scale;	// 描画倍率
	};

	// コンストラクタ
	AttackBase(const AttackBase::AttackParam&, const VECTOR& startPos, const VECTOR& dir);

	// デストラクタ
	~AttackBase(void) override;

	// 更新
	void Update(void) override;

	// 描画
	void Draw(void) override;

	// 解放
	void Release(void) override;

	// 死亡フラグ : ゲッター
	bool IsDead(void) const { return isDead_; }

	// オブジェクトマネージャのセッター
	void SetObjectManager(ObjectManager* objMng) { objMng_ = objMng; }

	// リソースロード
	void InitLoad(void) override {};

	// 大きさ、回転、座標の初期化
	void InitTransform(void) override {};

	// 衝突判定の初期化
	void InitCollider(void) override;

	// アニメーションの初期化
	void InitAnimation(void) override {};

	// 初期化後の個別処理
	void InitPost(void) override;

private:

	// オブジェクトマネージャ
	ObjectManager* objMng_;

	// オブジェクトデータ
	AttackParam param_;

	// 状態管理
	int stateBase_;

	// 状態管理(状態遷移時初期処理)
	std::map<int, std::function<void(void)>> stateChanges_;

	// 状態管理(更新ステップ)
	std::function<void(void)> stateUpdate_;

	// 状態遷移
	void ChangeState(int state);

	// 識別用ID
	int id;

	// 種別
	TYPE type_;

	// 移動速度float 
	float speed_;	

	// 最大移動距離(消滅しない距離)
	float maxDistance_;	

	// 描画倍率
	float scale_;

	// 攻撃オブジェクト初期位置
	VECTOR startPos_;

	// 攻撃オブジェクト移動方向
	VECTOR moveDir_;

	// 重力加速度
	float gravityVelocity_;

	// 描画用モデルハンドル
	int modelHandle_;

	// 攻撃管理フラグ
	bool isAttack_;

	// 死亡フラグ
	bool isDead_;

	// 消滅フェード中かどうかのフラグ
	bool isFading_;   

	// フェードが始まってからの経過時間
	float fadeTimer_;

	// 不透明度（1.0：くっきり ? 0.0：完全透明）
	float alpha_;

	// 攻撃処理関数
	void ProcessFireBall(void);
	void ProcessWaveAttack(void);
	void ProcessArrowAttack(void);
	void ProcessChargeAttack(void);
	void ProcessAxeThrowAttack(void);

};
