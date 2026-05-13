#pragma once
#include <functional>
#include "../CharactorBase.h"
class Player;

class BossBase : public CharactorBase
{
public:

	// 種別
	enum class BOSS_TYPE
	{
		PIXIE,
		ORC,
		DIABLO,
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
	static constexpr float VIEW_RANGE = 800.0f;

	// コンストラクタ
	BossBase(const BossBase::BossData& data);

	// デストラクタ
	virtual ~BossBase(void) override;

	void SetPlayer(Player* player);

protected:

	Player* player_;

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

	// 更新ステップ
	float step_;

	// ボス通知フラグ
	bool isUnaware_;	// 未発見
	bool isAlerted_;	// 発見時
	bool isEngaged_;	// 発見後
	bool isSearching_;	// 捜索フラグ

	// 更新系
	virtual void UpdateProcessPost(void) override {}

	// 索敵
	virtual void Search(void) = 0;

	// プレイヤーを注視する
	virtual void LookPlayer(void) = 0;

	// 視野描画
	virtual void DrawViewRange(void) override {}


};
