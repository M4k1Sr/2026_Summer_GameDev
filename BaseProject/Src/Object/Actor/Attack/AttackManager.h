#pragma once
#pragma once
#include <vector>
#include <memory>
#include <DxLib.h>
class ObjectManager;
class AttackBase;
class BossBase;
class ColliderBase;

class AttackManager
{
public:

	// オブジェクト生成のためにもらう
	AttackManager(ObjectManager* objMng);
	~AttackManager();

	// 初期化
	void Init(void);

	// 更新
	void Update(void);

	// 描画
	void Draw(void);

	// 解放
	void Release(void);

	// CSVからオブジェクト情報の読取を行う
	void LoadCsvData(void);

	// 技の生成関数（ここにボスやプレイヤーから呼ばれる関数を追加していく）
	void SpawnFireBall(const VECTOR& startPos, const VECTOR& dir);

	// オブジェクト
	const std::vector<AttackBase*>& GetAttacks(void) const { return attacks_; }

	// 衝突対象となるコライダを登録
	void AddHitCollider(const ColliderBase* hitCollider);

private:

	// オブジェクトマネージャ
	ObjectManager* objMng_;

	// アタックオブジェクト
	std::vector<AttackBase*> attacks_;


};

