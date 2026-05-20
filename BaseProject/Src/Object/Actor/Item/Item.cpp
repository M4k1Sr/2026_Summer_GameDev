#include "Item.h"
#include "../../../Manager/ResourceManager.h"
#include "../../../Manager/SceneManager.h"
#include "../../../Utility/AsoUtility.h"

/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="type">アイテムの種類（KEYなど）</param>
/// <param name="position">ボスが倒れた瞬間の座標</param>
Item::Item(ItemManager::ITEM_TYPE type, VECTOR position)
	:
	type_(type),
	isDead_(false),
	angleY_(0.0f)
{
	// 敵が死んだ位置を初期座標として設定
	transform_.pos = position;

	// ドロップした瞬間に上方向（Y軸）へ少し飛び出す初速度を設定
	// 放物線（ピョコッと跳ねる演出）を描かせるための力です
	velocity_ = VGet(0.0f, 5.0f, 0.0f);
}

/// <summary>
/// 初期化処理
/// </summary>
void Item::Init(void)
{
	// アイテムの種類が「鍵」の場合、リソースマネージャーからモデルを設定
	if (type_ == ItemManager::ITEM_TYPE::KEY)
	{
		auto& resMng = ResourceManager::GetInstance();

		// 既存のResourceManagerに設定した鍵（ITEM_KEY等）のモデルIDをセット
		// ※ResourceManager::SRC にアイテム用の列挙型を追加して合わせてください
		transform_.SetModel(resMng.Load(ResourceManager::SRC::KEY).handleId_);
	}

	// 大きさを等倍に設定
	transform_.scl = VGet(1.0f, 1.0f, 1.0f);

	// 回転を初期状態（回転なし）にリセット
	transform_.quaRot = Quaternion::Identity();

	// 最初のトランスフォーム（行列）を確定させる
	transform_.Update();
}

/// <summary>
/// 毎フレームの更新処理
/// </summary>
void Item::Update(void)
{
	// すでにプレイヤーに拾われるなどして死亡している場合は処理しない
	if (isDead_) return;

	// デルタタイム（前フレームからの経過時間）を取得
	auto& scnMng = SceneManager::GetInstance();
	float deltaTime = scnMng.GetDeltaTime();

	// --- 1. ドロップ時の物理挙動（放物線）の計算 ---
	// 重力加速度を毎フレーム下方向（マイナス）に加算していく
	float gravity = -9.8f * deltaTime;
	velocity_.y += gravity;

	// 計算した速度を元に座標を移動させる
	transform_.pos = VAdd(transform_.pos, VScale(velocity_, deltaTime));

	// 地面との簡易的な着地判定（高さをボスの足元付近の 0.0f とする例）
	if (transform_.pos.y < 0.0f)
	{
		transform_.pos.y = 0.0f;       // 地面に固定
		velocity_ = AsoUtility::VECTOR_ZERO; // 着地したので移動速度をゼロにする
	}

	// --- 2. アイテムの回転アニメーション演出 ---
	// ゲームのアイテムらしく見せるため、Y軸を中心に時間経過でくるくる回す
	angleY_ += 2.0f * deltaTime;
	transform_.quaRot = Quaternion::Euler(VGet(0.0f, angleY_, 0.0f));

	// 変更した座標や回転を行列に適用し、DxLibに反映させる
	transform_.Update();
}

// 描画処理
void Item::Draw(void)
{
	// 死亡（消滅）している場合は画面に描画しない
	if (isDead_) return;

	// トランスフォームに有効なモデルIDがセットされていれば、DxLibの機能で描画する
	if (transform_.modelId != -1)
	{
		MV1DrawModel(transform_.modelId);
	}
}

VECTOR Item::GetPos(void)
{
	return transform_.pos;
}

ItemManager::ITEM_TYPE Item::GetType(void)
{
	return type_;
}

void Item::Kill(void)
{
	isDead_ = true;
}

bool Item::IsDead(void) const
{
	return isDead_;
}
