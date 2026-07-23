#pragma once
#include "SceneBase.h"
#include "../Framework.h"
class IronBall;
class Clock;
class StageBase;
class Stage;
class SkyDome;
class Player;
class BossManager;
class ObjectManager;
class AttackManager;
class ItemManager;
class Ranking;
class ItemManager;
class SoundManager;
class UIManager;
class EffectManager;
class Transform;

class GameScene : public SceneBase
{

public:
	
	// コンストラクタ
	GameScene(void);

	// デストラクタ
	~GameScene(void) override;

	// 初期化
	void Init(void) override;

	// 更新
	void Update(void) override;

	// 描画
	void Draw(void) override;

	// 解放
	void Release(void) override;

	// ポーズ判定
	void IsPause(void);

	//スコア関連
	void Score(void);

	//スコア受け渡し
	int GetScore(void);

	//ゲームクリア判定
	void IsClear(void);

private:

	// ステージ状態
	enum class StageState
	{
		STAGE_1,
		STAGE_2,
	};

	// エフェクトマネージャ
	EffectManager* effectMng_;

	// サウンドマネージャ
	SoundManager* soundMng_;

	// UIマネージャ
	UIManager* UIMng_;

	// フェード状態
	enum class FadeState
	{
		NONE,       // フェードしていない（通常時）
		FADE_OUT,   // 暗転中
		FADE_IN     // 明転中
	};

	// ステージ状態
	StageState stageState_;

	//フェード管理
	FadeState fadeState_ = FadeState::NONE;

	// ステージ
	StageBase* stage_;

	// スカイドーム
	SkyDome* skyDome_;

	// プレイヤー
	Player* player_;
	
	// ボス
	BossManager* bossMng_;
	
	// オブジェクト
	ObjectManager* objMng_;

	// 攻撃処理
	AttackManager* attackMng_;

	// アイテム
	ItemManager* itemMng_;

	//ランキング
	Ranking* rank_;

	// 鉄球
	IronBall* ironBall_;
	
	//時計のUI
	Clock* clockUI_;
<<<<<<< HEAD



	
	// 時計
	//Transform clock_;
=======
>>>>>>> c705d334f18ebf93ee247c6d6c4234148284325c
	
	bool isPause_;
	int pauseImg_;
	bool isSousa_;
	int sousaImg_;
	int mosPosX_, mosPosY_;
	int currentStageNum_;
	int fadeAlpha_;
	int fadeSpeed_;

	static constexpr int DRAWBOX_SX = 400;
	static constexpr int DRAWBOX_EX = 1600;
	static constexpr int DRAWBOX_GAMEEND_SY = 600;
	static constexpr int DRAWBOX_GAMEEND_EY = 800;
	static constexpr int DRAWBOX_GAME_SY = 200;
	static constexpr int DRAWBOX_GAME_EY = 400;

	//ゲーム終了判定
	bool isEnd_;

	//ゲームクリア判定
	bool isClear_;

	//クリアタイム
	float clearTime_;

	//デバッグ用ゴール画像
	int goalImg_;

	// フェード処理用のプライベート関数群
	void StartFade(FadeState state, int speed = 5); // フェード開始トリガー
	bool UpdateFade(void);                          // フェードの更新（完了したらtrueを返す）
	void DrawFade(void) const;

};
