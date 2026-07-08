#pragma once
#include <map>
#include <string>
#include "Resource.h"

class ResourceManager
{

public:

	// リソース名
	enum class SRC
	{
		TITLE,
		PUSH_SPACE,
		PLAYER,
		FIRST_STAGE,
		SECOND_STAGE,
		SKY_DOME,
		PLAYER_SHADOW,

		// オブジェクト
		OBJECT_BOX,
		OBJECT_TILE,
		IRON_BALL,
		OBJECT_SWITCH,
		TARAI,
		NEEDLE_FLOOR,
		BURNER,
		CONVEYER,
		CAGE,
		BREAK_CAGE,

		// UI
		CLOCK,
		CLOCK_HAND,

		// エネミー
		BOSS_PIXIE,
		BOSS_GOBLIN,


		KEY,         //アイテム鍵
		WALL,
		GOAL,
		DAMAGE,//ダメージエフェクト
		TIMER, //タイマーエフェクト
		SWEAT,
		IRONBALL_HIT,	// 鉄球エフェクト
		PIXIE_FIRE,		// ピクシー炎攻撃
		PIXIE_WAVE,		// ピクシー波状攻撃

		// 武器オブジェクト
		CLUB,


	};

	// 明示的にインステンスを生成する
	static void CreateInstance(void);

	// 静的インスタンスの取得
	static ResourceManager& GetInstance(void);

	// 初期化
	void Init(void);

	// 解放(シーン切替時に一旦解放)
	void Release(void);

	// リソースの完全破棄
	void Destroy(void);

	// リソースのロード
	const Resource& Load(SRC src);

	// リソースの複製ロード(モデル用)
	int LoadModelDuplicate(SRC src);


private:

	// 静的インスタンス
	static ResourceManager* instance_;

	// リソース管理の対象
	std::map<SRC, Resource*> resourcesMap_;

	// 読み込み済みリソース
	std::map<SRC, Resource&> loadedMap_;

	Resource dummy_;

	// デフォルトコンストラクタをprivateにして、
	// 外部から生成できない様にする
	ResourceManager(void);
	ResourceManager(const ResourceManager& manager) = default;
	~ResourceManager(void) = default;

	// 内部ロード
	Resource& _Load(SRC src);

};
