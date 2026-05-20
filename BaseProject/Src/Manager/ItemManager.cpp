#include "ItemManager.h"
#include "../Object/Actor/Item/Item.h" 

// 静的メンバ変数の実体定義
ItemManager* ItemManager::instance_ = nullptr;

// コンストラクタ
ItemManager::ItemManager(void)
{
    // 必要ならメンバ変数の初期化
}

// 明示的にインスタンスを生成する
void ItemManager::CreateInstance(void)
{
    if (instance_ == nullptr)
    {
        instance_ = new ItemManager();
    }
}

// 静的インスタンスの取得
ItemManager& ItemManager::GetInstance(void)
{
    return *instance_;
}

// 初期化
void ItemManager::Init(void)
{
    items_.clear();
}

// 更新
void ItemManager::Update(void)
{
    // 各アイテムの更新処理
    for (auto& item : items_)
    {
        item->Update();
    }

    // 死亡フラグ（消滅フラグ）が立っているアイテムをリストから削除する処理
    // ※Itemクラスに IsDead() などの関数がある想定
    
    items_.erase(
        std::remove_if(items_.begin(), items_.end(), [](const std::shared_ptr<Item>& item) {
            return item->IsDead();
        }),
        items_.end()
    );
 
}

// 描画
void ItemManager::Draw(void)
{
    // 各アイテムの描画処理
    for (auto& item : items_)
    {
        item->Draw();
    }
}

// 解放（シーン切り替え時など）
void ItemManager::Release(void)
{
    items_.clear(); 
}

// リソースの完全破棄
void ItemManager::Destroy(void)
{
    if (instance_ != nullptr)
    {
        delete instance_;
        instance_ = nullptr;
    }
}

// アイテムの生成
void ItemManager::SpawnItem(ITEM_TYPE type, VECTOR position)
{
     auto newItem = std::make_shared<Item>(type, position);
     items_.push_back(newItem);
}

// ゲッター
const std::vector<std::shared_ptr<Item>>& ItemManager::GetItems(void) const
{
    return items_;
}