#pragma once
#include "../ActorBase.h"
#include "../../../Object/Collider/ColliderModel.h"
#include <string>
#include <vector>

class StageBase : public ActorBase
{

public:

    // 衝突判定種別（すべてのステージ共通）
    enum class COLLIDER_TYPE {
        MODEL = 0,
        MAX,
    };

    //コンストラクタ・デストラクタ
    StageBase(void);
    virtual ~StageBase(void) override;

    // 共通の更新・描画・解放処理
    virtual void Update(void) override;
    virtual void Draw(void) override;
    virtual void Release(void) override;

protected:

    // 各ステージで固有の処理を実装してもらうための純粋仮想関数
    virtual void InitLoad(void) override = 0;
    virtual void InitTransform(void) override = 0;
    virtual void InitCollider(void) override = 0;

    // アニメーションやBGMは空の関数にしておき、必要に応じて子クラスでオーバーライドさせる
    virtual void InitAnimation(void) override {}
    virtual void InitPost(void) override {}

    // 子クラスからアクセスしやすいようにコライダー設定用の共通関数を作っておく
    void SetupModelCollider(ColliderBase::TAG tag, const std::vector<std::string>& excludeFrames, const std::vector<std::string>& targetFrames);
};