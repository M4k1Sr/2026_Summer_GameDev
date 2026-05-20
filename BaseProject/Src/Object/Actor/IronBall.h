#pragma once
#include <DxLib.h>
#include <string>
#include "ActorBase.h"

class IronBall : public ActorBase
{
public:

	// 衝突判定種別
	enum class COLLIDER_TYPE
	{
		MODEL = 0,
		MAX,
	};

	// コンストラクタ
	IronBall(void);

	// デストラクタ
	~IronBall(void) override;

	// 更新
	void Update(void) override;
	// 描画
	void Draw(void) override;
	// 解放
	void Release(void) override;

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

private:

	// 各個体の位置情報を管理する構造体
	struct InstanceData {

		// 初期の支点座標
		VECTOR basePos;    

		// 現在の座標・回転・モデルID
		Transform transform;

		// 揺れるタイミングをずらすためのオフセット
		float timeOffset; 

		// 個体ごとのモデルコライダー
		class ColliderModel* colModel = nullptr;
	};

	// 複数の鉄球データ
	std::vector<InstanceData> instances_; 

	// 振り子計算（個別のデータを渡すように変更）
	void Pendulum(InstanceData& data);

	// 描画したい個数
	static constexpr int INSTANCE_COUNT = 3;   

	// 横の間隔
	static constexpr float INTERVAL_X = 300.0f;      

	// 鉄球の座標
	static constexpr VECTOR IRON_BALL_POS = { 330.0f, 275.0f, 0.0f };

	// 鎖の先端の座標
	static constexpr float CHAIN_END_POS = 130.0f;

	//鉄球の大きさ
	static constexpr VECTOR IRON_BALL_SCALE = { 0.6f, 0.6f, 0.6f };

	// コライダ関連（ヘッダに置いて問題ありません。静的定数として定義）
	// ローカル基準で下方向のオフセット（負値）
	static constexpr float COLLIDER_LOCAL_OFFSET_Y = -240.0f;
	
	// 基本半径（transform.scl.x と乗算）
	static constexpr float BASE_COLLIDER_RADIUS = 200.0f;

	// 除外フレーム名称
	const std::vector<std::string> EXCLUDE_FRAME_NAMES = {
	"Torus",
	};

	// 対象フレーム
	const std::vector<std::string> TARGET_FRAME_NAMES = {
	"Ball",
	};
};

