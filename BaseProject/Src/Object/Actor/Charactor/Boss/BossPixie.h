#pragma once
#include <DxLib.h>
#include "BossBase.h"
#include "../CharactorBase.h"
class AnimationController;

class BossPixie : public BossBase
{
public:

	// アニメーション種別
	enum class ANIM_TYPE
	{
		IDLE,
		SURPRISE,
		CHARGE,
		THROW,
		ATTACK_WAVE,
		ATTACK_END,
		DAMAGE,
		DOWN,
		END,
		MAX,
	};

	// 状態
	enum class STATE
	{
		IDLE,
		SURPRISE,
		CHARGE,
		THROW,
		ATTACK_WAVE,
		ATTACK_END,
		DAMAGE,
		DOWN,
		END,
	};

	// コンストラクタ
	BossPixie(const BossBase::BossData& data);

	// デストラクタ
	~BossPixie(void) override;


protected:

	// リソースロード
	void InitLoad(void) override;

	// 大きさ、回転、座標の初期化
	void InitTransform(void) override;

	// 衝突判定の初期化
	void InitCollider(void) override;

	// アニメーションの初期化
	void InitAnimation(void) override;

	// 初期化後の個別処理
	void InitPost(void) override;

	// 更新系
	void UpdateProcess(void) override;
	void UpdateProcessPost(void) override;

private:

	// モデルの大きさ
	static constexpr float SCALE = 3.0f;
	
	// モデルのローカル回転
	static constexpr VECTOR ROT = { 0.0f, 180.0f * DX_PI_F / 90.0f, 0.0f };

	// 衝突判定用線分開始
	static constexpr VECTOR COL_LINE_START_LOCAL_POS = { 0.0f, 80.0f, 0.0f };

	// 衝突判定用線分終了
	static constexpr VECTOR COL_LINE_END_LOCAL_POS = { 0.0f, -10.0f, 0.0f };

	// 衝突判定用カプセル上部球体
	static constexpr VECTOR COL_CAPSULE_TOP_LOCAL_POS = { 0.0f, 110.0f, 0.0f };

	// 衝突判定用カプセル下部球体
	static constexpr VECTOR COL_CAPSULE_DOWN_LOCAL_POS = { 0.0f, 30.0f, 0.0f };

	// 衝突判定用カプセル球体半径
	static constexpr float COL_CAPSULE_RADIUS = 20.0f;

	// 衝突判定用カプセル球体半径
	static constexpr float STATE_THROW_TIME = 20.0f;

	// 衝突判定用カプセル球体半径
	static constexpr float STATE_ATTACK_WAVE_TIME = 10.0f;

	// 状態
	STATE state_;

	void ChangeState(STATE state);
	void ChangeStateIdle(void);
	void ChangeStateSurprise(void);
	void ChangeStateCharge(void);
	void ChangeStateThrow(void);
	void ChangeStateAttackWave(void);
	void ChangeStateAttackEnd(void);
	void ChangeStateDamage(void);
	void ChangeStateDown(void);
	void ChangeStateEnd(void);

	void UpdateIdle(void);
	void UpdateSurprise(void);
	void UpdateCharge(void);
	void UpdateThrow(void);
	void UpdateAttackWave(void);
	void UpdateAttackEnd(void);
	void UpdateDamage(void);
	void UpdateDown(void);
	void UpdateEnd(void);

};
