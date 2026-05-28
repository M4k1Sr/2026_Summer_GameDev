#pragma once
#include "./CharactorBase.h"
class ObjectManager;

class Player : public CharactorBase
{
public:

	enum class ANIM_TYPE
	{
		IDLE,
		RUN,
		FAST_RUN,
		JUMP,
		PUSH,
		MAX,
	};

	// コンストラクタ
	Player(void);

	// デストラクタ
	virtual ~Player(void) override;

	// 描画
	void Draw(void) override;

	// 解放
	void Release(void) override;

	// オブジェクトマネージャーのセット
	void SetObjectManager(ObjectManager* manager) { objMng_ = manager; }

	//プレイヤー座標のゲッター
	bool GetDeadFlag(void);

	void playerDead(void);

	// ダメージカウンタゲッター
	int GetCurrentCnt(void) const;

	//ゲームクリア判定
	bool GetClearFlag(void) const;

	void IsClear(void);

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
	virtual void UpdateProcess(void) override;
	virtual void UpdateProcessPost(void) override;


	// 視野描画
	virtual void DrawViewRange(void) override;

private:

	// プレイヤー
	Transform player_;

	// オブジェクトマネージャー
	ObjectManager* objMng_ = nullptr;

	// ギミック動作カウンタ
	float gimmickCnt_;
	bool isGimmick_;

	// ギミック動作数カウンタ
	int currentCnt_;

	// プレイヤー座標
<<<<<<< HEAD
	//static constexpr VECTOR PLAYER_POS = { 1000.0f, 0.0f, 880.0f };
	//static constexpr VECTOR PLAYER_POS = { -800.0f, 0.0f, 700.0f };
	static constexpr VECTOR PLAYER_POS = { 2000.0f, 0.0f, -750.0f };

=======
	static constexpr VECTOR PLAYER_POS = { 1000.0f, 0.0f, 880.0f };
	//static constexpr VECTOR PLAYER_POS = { 3600.0f, -98.0f, -800.0f };//{ 0.0f, 0.0f, -800.0f };
>>>>>>> 5058b032279c409eabf00936443c2f5a982e1e47
	// プレイヤースケール
	static constexpr float PLAYER_SCALE = 1.0f;
	// プレイヤーローカル角度
	static constexpr VECTOR PLAYER_LOCAL_ROT = { 0.0f, 180.0f * DX_PI_F / 180.0f, 0.0f };

	// 移動速度(通常)
	static constexpr float SPEED_MOVE = 5.0f;
	// 移動速度(ダッシュ)
	static constexpr float SPEED_DASH = 15.0f;

	// 衝突判定用線分開始
	static constexpr VECTOR COL_LINE_START_LOCAL_POS = { 0.0f, 80.0f, 0.0f };
	// 衝突判定用線分終了
	static constexpr VECTOR COL_LINE_END_LOCAL_POS = { 0.0f, -10.0f, 0.0f };

	// 衝突判定用線分開始(ジャンプ時)
	static constexpr VECTOR COL_LINE_JUMP_START_LOCAL_POS =
	{ 0.0f, 130.0f, 0.0f };

	// 衝突判定用線分終了(ジャンプ時)
	static constexpr VECTOR COL_LINE_JUMP_END_LOCAL_POS =
	{ 0.0f, 50.0f, 0.0f };

	// 衝突判定用カプセル上部球体
	static constexpr VECTOR COL_CAPSULE_TOP_LOCAL_POS = { 0.0f, 110.0f, 0.0f };
	// 衝突判定用カプセル下部球体
	static constexpr VECTOR COL_CAPSULE_DOWN_LOCAL_POS = { 0.0f, 30.0f, 0.0f };

	// 衝突判定用カプセル球体半径
	static constexpr float COL_CAPSULE_RADIUS = 20.0f;	

	// 衝突判定用カプセル上部球体(ジャンプ時)
	static constexpr VECTOR COL_CAPSULE_TOP_JUMP_LOCAL_POS =
	{ 0.0f, 160.0f, 0.0f };
	// 衝突判定用カプセル下部球体(ジャンプ時)
	static constexpr VECTOR COL_CAPSULE_DOWN_JUMP_LOCAL_POS =
	{ 0.0f, 80.0f, 0.0f };

	// ジャンプ力
	static constexpr float POW_JUMP_INIT = 4000.0f; 

	// 持続ジャンプ力
	static constexpr float POW_JUMP_KEEP = 600.0f;

	// ジャンプ受付時間
	static constexpr float TIME_JUMP_INPUT = 0.3f;

	//// 操作
	void ProcessMove(void);
	void ProcessJump(void);

	// ギミック操作
	void ProcessPush(void);
	void ProcessCarry(void);

	// 衝突判定
	void CollisionReserve(void) override;

	//ゲームクリア判定用のフラグ
	bool isClear_;

};

