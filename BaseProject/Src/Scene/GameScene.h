#pragma once
#include "SceneBase.h"
#include "../Framework.h"
class IronBall;
class UI;
class Stage;
class SkyDome;
class Player;
class BossManager;
class ObjectManager;
class Ranking;
class ItemManager;

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

	//アイテムドロップ判定
	void ItemDrop(void);

private:

	// ステージ
	Stage* stage_;

	// スカイドーム
	SkyDome* skyDome_;

	// プレイヤー
	Player* player_;
	
	// ボス
	BossManager* bossMng_;
	
	// オブジェクト
	ObjectManager* objMng_;

	//ランキング
	Ranking* rank_;

	// 鉄球
	IronBall* ironBall_;
	
	//UI
	UI* ui_;

	//Item
	ItemManager* itemMng_;
	
	// 時計
	Transform clock_;
	
	bool isPause_;
	int pauseImg_;
	bool isSousa_;
	int sousaImg_;
	int mosPosX_, mosPosY_;

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

	//スコア
	int score_;

	//クリアタイム
	float clearTime_;

};
