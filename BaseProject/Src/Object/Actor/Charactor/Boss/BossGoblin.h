#include "BossBase.h"
#include "../CharactorBase.h"
class Health;
class AnimationController;

class BossGoblin : public BossBase
{
public:

	// アニメーション種別
	enum class ANIM_TYPE
	{
		// 通常状態
		IDLE,
		WALK,
		RUN,
		PATROL,

		// 発見状態
		SURPRISE,	// 発見
		THREAT,		// 威嚇

		// 攻撃状態
		ATTACK,
		THROW,
		ATTACK_END,

		// 攻撃後感情
		CHEER,
		ANGRY,

		// ダメージ状態
		DAMAGE,
		DOWN,
		END
	};



	// コンストラクタ
	BossGoblin(const BossBase::BossData& data);

	// デストラクタ
	virtual ~BossGoblin();



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

	// HP管理
	Health* health_;

	// ダメージカウンタ
	int lastDamageCnt_ = 0;

	// 攻撃タイマー
	float attackTimer_;

	// モデルの大きさ
	static constexpr float SCALE = 2.0f;

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

	// 状態遷移
	void ChangeState(STATE state);
	
	// 通常状態
	void ChangeStateIdle(void);
	void ChangeStateWalk(void);
	void ChangeStateRun(void);
	void ChangeStatePatrol(void);

	// 発見状態
	void ChangeStateSurprise(void);
	void ChangeStateThreat(void);

	// 攻撃状態
	void ChangeStateAttack(void);
	void ChangeStateThrow(void);
	void ChangeStateAttackEnd(void);

	// 攻撃後感情
	void ChangeStateCheer(void);
	void ChangeStateAngry(void);

	// ダメージ状態
	void ChangeStateDamage(void);
	void ChangeStateDown(void);
	void ChangeStateEnd(void);


	// 更新処理
	// 通常状態
	void UpdateIdle();
	void UpdateWalk();
	void UpdateRun();
	void UpdatePatrol();

	// 発見状態
	void UpdateSurprise();
	void UpdateThreat();

	// 攻撃状態
	void UpdateAttack();
	void UpdateThrow();
	void UpdateAttackEnd();

	// 攻撃後感情
	void UpdateCheer();
	void UpdateAngry();

	// ダメージ状態
	void UpdateDamage();
	void UpdateDown();
	void UpdateEnd();

	// フェーズ管理
	void Phase(void) override;

	void Dead(void);

	bool isDead_;

};
