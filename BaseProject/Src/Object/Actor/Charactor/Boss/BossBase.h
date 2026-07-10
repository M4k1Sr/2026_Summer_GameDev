#pragma once
#include <functional>
#include <memory>
#include "../CharactorBase.h"
class Player;
class ObjectManager;
class StrategyAttack;
class AttackManager;

class BossBase : public CharactorBase
{
public:

	// 種別
	enum class BOSS_TYPE
	{
		PIXIE,
		GOBLIN,
		DIABLO,
	};

	// ボスのフェーズ状態
	enum class PHASE_STEP
	{
		PHASE_IDLE,	// 初期
		PHASE_ENCOUNT,// 序盤
		PHASE_TACTICAL,// 中盤
		PHASE_CLIMAX,// 終盤
		PHASE_DEAD,	// 決着
	};

	// 状態
	enum class STATE
	{
		// 通常状態
		IDLE,
		YAWN,
		IDLE_JUMP,
		SIT,

		// 移動状態
		WALK,
		RUN,
		PATROL,

		// 発見状態
		SURPRISE,	// 発見
		THREAT,		// 威嚇

		// 攻撃状態
		CHARGE,
		ATTACK,
		THROW,
		ATTACK_WAVE,
		ATTACK_END,

		// 攻撃後感情
		CHEER,
		ANGRY,

		// ダメージ状態
		DAMAGE,
		DOWN,
		END,
	};

	// オブジェクトデータ
	struct BossData
	{
		int id;
		BossBase::BOSS_TYPE type;
		VECTOR defaultPos;
	};

	// 視野角
	static constexpr float VIEW_ANGLE = 80.0f;

	// 視野の広さ
	static constexpr float VIEW_RANGE = 2000.0f;

	// コンストラクタ
	BossBase(const BossBase::BossData& data);

	// デストラクタ
	virtual ~BossBase(void) override;

	void SetPlayer(Player* player);

	// オブジェクトマネージャーのセット
	void SetObjectManager(ObjectManager* manager) { objMng_ = manager; }

	Player* GetPlayer() const { return player_; }

	ObjectManager* GetObjectManager() const { return objMng_; }

	VECTOR GetPos() const { return transform_.pos; }

	// これが「ChangeAttackStrategy」の正体です！
	void ChangeAttackStrategy(std::unique_ptr<StrategyAttack> newStrategy); 

	// 攻撃マネージャーのセット
	void SetAttackManager(AttackManager* manager) { attackMng_ = manager; }

	// 攻撃マネージャーのゲッター
	AttackManager* GetAttackManager() const { return attackMng_; }

	// ボスが死亡しているかどうかを取得する関数を追加
	bool GetIsDead(void) const { return phaseStep_ == PHASE_STEP::PHASE_DEAD; }

protected:

	Player* player_;

	// オブジェクトマネージャー
	ObjectManager* objMng_ = nullptr;

	// 攻撃マネージャー
	AttackManager* attackMng_ = nullptr;

	// 攻撃をしまっておくポケット
	// 攻撃関数を入れておくボックスのようなもの(火の玉攻撃や斧攻撃など)
	std::unique_ptr<StrategyAttack> currentAttack_;

	// 状態管理
	int stateBase_;

	// 状態管理(状態遷移時初期処理)
	std::map<int, std::function<void(void)>> stateChanges_;

	// 状態管理(更新ステップ)
	std::function<void(void)> stateUpdate_;

	// リソースロード
	void InitLoad(void) override {}

	// 大きさ、回転、座標の初期化
	void InitTransform(void) override {}

	// 衝突判定の初期化
	void InitCollider(void) override {}

	// アニメーションの初期化
	void InitAnimation(void) override {}

	// 初期化後の個別処理
	void InitPost(void) override {}

	// 状態遷移
	void ChangeState(int state);

	// 種別
	BOSS_TYPE type_;

	// 状態遷移時間
	float stateTime_;

	// 経過時間
	float stateTimer_;

	// IDLEタイマー
	float idleTimer_;

	// ボス通知フラグ
	bool isUnaware_;	// 未発見:true	発見後:false
	bool isAlerted_;	// 発見時:true
	bool isEngaged_;	// 発見後:true
	bool isSearching_;	// 捜索フラグ:true

	// 攻撃フラグ
	bool isAttack_;

	// 攻撃可能フラグ
	bool doAttack_;

	// 状態
	STATE state_;

	// フェーズ状態
	PHASE_STEP phaseStep_;

	
	// 更新系
	virtual void UpdateProcessPost(void) override {}

	// 索敵
	virtual void Search(void) = 0;

	// プレイヤーを注視する
	virtual void LookPlayer(void) = 0;

	// 視野描画
	virtual void DrawViewRange(void) override {}

	// フェーズ管理
	virtual void Phase(void) = 0;

private:


};
