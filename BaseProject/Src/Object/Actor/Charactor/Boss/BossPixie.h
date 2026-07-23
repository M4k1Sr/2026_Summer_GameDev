#pragma once
#include <DxLib.h>
#include <memory>
#include "BossBase.h"
#include "../CharactorBase.h"
class Health;
class AnimationController;
class ModelMaterial;
class ModelRenderer;


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



	// コンストラクタ
	BossPixie(const BossBase::BossData& data);

	// デストラクタ
	virtual ~BossPixie();

	void Draw(void) override;


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

	// 視野描画
	void DrawViewRange(void) override;

private:

	// 
	// 管理
	Health* health_;

	//モデルレンダラー
	std::unique_ptr<ModelRenderer> renderer_;

	//モデルマテリアル
	std::unique_ptr<ModelMaterial> material_;

	// ダメージカウンタ
	int lastDamageCnt_ = 0;

	// 火の玉攻撃カウンタ
	int throwCnt_;

	// 攻撃波攻撃カウンタ
	int waveAttackCnt_;
	
	// 攻撃タイマー
	float attackTimer_;

	// 経過時間
	float timer_ = 0.0f;

	// 消えるまでのフレーム数（例: 2秒なら120）
	float duration_ = 300.0f;

	// モデルの大きさ
	static constexpr float SCALE = 3.0f;
	
	// モデルのローカル回転
	static constexpr VECTOR ROT = { 0.0f, 180.0f * DX_PI_F / 180.0f, 0.0f };

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

	// 索敵
	void Search(void);
	// プレイヤーを注視する
	void LookPlayer(void);

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

	// フェーズ管理
	void Phase(void) override;

	
	//死亡処理
	void Dead(void);

	bool isDead_;

};
