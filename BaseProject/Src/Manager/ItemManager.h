#pragma once
#include <vector>
#include <memory>
#include <DxLib.h> 
class Item;

class ItemManager
{
public:

	// アイテムの種類
	enum class ITEM_TYPE
	{
		NONE,
		KEY,       //鍵
	};

	// デフォルトコンストラクタをprivateにして、
// 外部から生成できない様にする
	ItemManager(void);
	ItemManager(const ItemManager& manager) = default;
	~ItemManager(void) = default;



	// 明示的にインスタンスを生成する
	static void CreateInstance(void);

	// 静的インスタンスの取得
	static ItemManager& GetInstance(void);

	// 初期化
	void Init(void);

	// 更新（毎フレームの移動処理や消滅フラグの監視）
	void Update(void);

	// 描画
	void Draw(void);

	// 解放（シーン切り替え時などにアイテムを全削除）
	void Release(void);

	// リソースの完全破棄（インスタンス自体の削除）
	void Destroy(void);

	/// <summary>
	/// 指定した座標にアイテムを生成する（ボス死亡時などに呼び出す）
	/// </summary>
	/// <param name="type">アイテムの種類</param>
	/// <param name="position">生成する3D座標</param>
	void SpawnItem(ITEM_TYPE type, VECTOR position);

	// 衝突判定用にアイテムリストを取得するゲッター（必要に応じて）
	const std::vector<std::shared_ptr<Item>>& GetItems(void) const;

private:

	// 静的インスタンス
	static ItemManager* instance_;

	// 管理しているアイテムのリスト
	std::vector<std::shared_ptr<Item>> items_;



};