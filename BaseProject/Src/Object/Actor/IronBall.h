#pragma once
#include <DxLib.h>
#include <string>
#include "ActorBase.h"
class ColliderSphere;

class IronBall : public ActorBase
{
public:

	// 衝突判定種別
	enum class COLLIDER_TYPE
	{
		SPHERE = 0,
		MAX,
	};

	// コンストラクタ
	IronBall(void);

	// デストラクタ
	~IronBall(void) override;

	// 更新、描画、解放
	void Update(void) override;
	void Draw(void) override;
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

	// 描画したい個数
	static constexpr int INSTANCE_COUNT = 3;

	// 横の間隔
	static constexpr float INTERVAL_X = 300.0f;

<<<<<<< HEAD
	// 鉄球の座標
	static constexpr VECTOR IRON_BALL_POS = { 1650.0f, 400.0f, -1200.0f };

	// 鉄球の座標f
	//static constexpr VECTOR IRON_BALL_POS = { 700.0f, 400.0f, 1630.0f };
=======
	// 鉄球の座標f
	static constexpr VECTOR IRON_BALL_POS = { 700.0f, 400.0f, 1630.0f };
>>>>>>> c705d334f18ebf93ee247c6d6c4234148284325c

	// 鎖の先端の座標
	static constexpr float CHAIN_END_POS = 160.0f;

	//鉄球の大きさ
	static constexpr VECTOR IRON_BALL_SCALE = { 0.6f, 0.6f, 0.6f };

	// === 球体コライダー用のパラメータ定数 ===
	// 鉄球の中心（ローカル座標）へのオフセット
	static constexpr VECTOR SPHERE_LOCAL_POS = { 0.0f, -240.0f, 0.0f };

	// 球体の半径
	static constexpr float SPHERE_RADIUS = 120.0f;

	// 各個体の位置情報を管理する構造体
	struct InstanceData {

		// 初期の支点座標
		VECTOR basePos;    

		// 現在の座標・回転・モデルID
		Transform transform;

		// 揺れるタイミングをずらすためのオフセット
		float timeOffset; 
	};

	// 振り子計算（個別のデータを渡すように変更）
	void Pendulum(InstanceData& data);

	// 複数の鉄球データ
	std::vector<InstanceData> instances_; 

	// 2個目以降の鉄球コライダーもすべて安全に保持・管理するための動的配列
	std::vector<ColliderSphere*> myColliders_;

};

