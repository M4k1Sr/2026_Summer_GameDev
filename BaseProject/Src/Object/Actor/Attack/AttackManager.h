#pragma once
#pragma once
#include <vector>
#include <memory>
#include <DxLib.h>
#include "./AttackBase.h"
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
	void Create(const AttackBase::AttackParam& data,const VECTOR& startPos, const VECTOR& dir);

	// オブジェクト
	const std::vector<AttackBase*>& GetAttacks(void) const { return attacks_; }

	// 衝突対象となるコライダを登録
	void AddHitCollider(const ColliderBase* hitCollider);

	// データ一行分取得
	const AttackBase::AttackParam& GetMasterData(AttackBase::TYPE type) const;

private:

	// オブジェクトマネージャ
	ObjectManager* objMng_;

	// アタックオブジェクト
	std::vector<AttackBase*> attacks_;

	// 攻撃オブジェクトデータ保管マップ
	std::map<AttackBase::TYPE, AttackBase::AttackParam> masterMap_;
};

