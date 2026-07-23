#pragma once
#include "./CharactorBase.h"
class ObjectManager;
class ItemManager;
class Health;

class Player : public CharactorBase
{
public:

	enum class ANIM_TYPE
	{
		IDLE,
		SLOW_WALK,
		RUN,
		FAST_RUN,
		JUMP,
		PUSH,
		HOLD,
		THROW,
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
	void SetObjectManager(ObjectManager* objMng) { objMng_ = objMng; }

	// アイテムマネージャーのセット
	void SetItemManager(ItemManager* itemMng) { itemMng_ = itemMng; }
	//プレイヤー座標のゲッター
	bool GetDeadFlag(void);

	void PlayerDead(void);

	// ダメージカウンタゲッター
	int GetCurrentCnt(void) const;

	//ゲームクリア判定
	bool GetClearFlag(void) const;

	// 1stStageクリア判定
	void IsClear(void);

	// ゲームクリア判定
	bool IsGameClear(void);

	// プレイヤーの向き
	VECTOR GetDir(void) const { return dir_; }
	void SetDir(VECTOR dir) { dir_ = dir; }

	//プレイヤーの位置変更
	void SetPosition(const VECTOR& newPos) { transform_.pos = newPos; }

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
	virtual void DrawViewRange(void) override {};

protected:
	

private:

	// プレイヤー
	Transform player_;

	// オブジェクトマネージャー
	ObjectManager* objMng_ = nullptr;

	// アイテムマネージャー
	ItemManager* itemMng_;

	// ボムデータ
	WeaponData bombData_;

	// HP管理
	Health* health_;

	// ギミック動作カウンタ
	float gimmickCnt_;
	bool isGimmick_;

	// ギミック動作数カウンタ
	int currentCnt_;

	// ダッシュスタミナ
	float stamina_;
	float maxStamina_;	// 最大スタミナ

	// ダッシュ判定
	bool isDash_;

	// 歩き判定
	bool isSlowWalk_;
		
	// 汗UIの座標
	VECTOR sweatPos_;

	// ボム座標
	VECTOR bombPos_;

	// プレイヤー座標
	//static constexpr VECTOR PLAYER_POS = { -700.0f, 50.0f, 750.0f };	// スタート位置

	//static constexpr VECTOR PLAYER_POS = { 1800.0f, 0.0f, -750.0f };	// ボススタート位置
	//static constexpr VECTOR PLAYER_POS = { 9800.0f, 50.0f, -1220.0f };
	//static constexpr VECTOR PLAYER_POS = { -700.0f, 50.0f, 750.0f };	// スタート位置
	//static constexpr VECTOR PLAYER_POS = { 9000.0f, 2.0f, -800.0f };	// ボススタート位置
	//static constexpr VECTOR PLAYER_POS = { 3600.0f, -98.0f, -800.0f };
	// static constexpr VECTOR PLAYER_POS = { -700.0f, 50.0f, 750.0f };	// スタート位置
	//static constexpr VECTOR PLAYER_POS = { 30000.0f, 0.0f, -3750.0f };	// ボススタート位置
	//static constexpr VECTOR PLAYER_POS = { 3600.0f, 50.0f, -800.0f };
	//static constexpr VECTOR PLAYER_POS = { -700.0f, 50.0f, -750.0f };	// スタート位置
	//static constexpr VECTOR PLAYER_POS = { 1500.0f, 50.0f, 9000.0f };	// ボス位置


	//static constexpr VECTOR PLAYER_POS = { 9000.0f, 2.0f, -800.0f };	// ボススタート位置
	//static constexpr VECTOR PLAYER_POS = { 3600.0f, -98.0f, -800.0f };

	// 向き
	VECTOR dir_;

	//// プレイヤー座標
	static constexpr VECTOR PLAYER_POS = { -700.0f, 50.0f, -750.0f };	// スタート位置
	//static constexpr VECTOR PLAYER_POS = { 1500.0f, 50.0f, 8000.0f };	// ボス位置

	// プレイヤースケール
	static constexpr float PLAYER_SCALE = 1.0f;

	// プレイヤーローカル角度
	static constexpr VECTOR PLAYER_LOCAL_ROT = { 0.0f, 180.0f * DX_PI_F / 180.0f, 0.0f };

	// 移動速度(通常)
	static constexpr float SPEED_MOVE = 4.0f;

	// 移動速度(ダッシュ)
	static constexpr float SPEED_DASH = 7.0f;

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
	static constexpr float POW_JUMP_INIT = 25.0f; 

	// 持続ジャンプ力
	static constexpr float POW_JUMP_KEEP = 600.0f;

	// ジャンプ受付時間
	static constexpr float TIME_JUMP_INPUT = 0.2f;

	// ダッシュスタミナ
	static constexpr float STAMINA_DASH_DECREASE = 5.0f;	// ダッシュスタミナ減少量

	// 爆弾モデルの大きさ
	static constexpr float BOMB_SCL = 2.0f;

	// 爆弾のローカル座標・回転
	static constexpr VECTOR BOMB_LOCAL_POS = { 0.0f, 0.0f, 0.0f };
	static constexpr VECTOR BOMB_LOCAL_ROT = { 90.0f * DX_PI_F / 180.0f,90.0f * DX_PI_F / 180.0f,0.0f };

	// UI初期化
	void InitUI(void);

	//// 操作
	void ProcessMove(void);
	void ProcessJump(void);

	// ギミック操作
	void ProcessPush(void);
	void ProcessThrow(void);

	// 衝突判定
	void CollisionReserve(void) override;

	// 1stStageクリア判定用のフラグ
	bool isClear_;

	// 2ndStageクリア判定用のフラグ
	bool gameClear_;

	//鉄球との衝突判定
	bool isIronBallHit_;
};

