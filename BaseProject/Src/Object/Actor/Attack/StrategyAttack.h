#pragma once
#include <memory>
class BossBase;

class StrategyAttack
{
public:

    virtual ~StrategyAttack() = default;

    // 実行関数（引数でボス自身を受け取ることで、ボスの座標やプレイヤーの情報を使えるようにする）
    virtual void ExecuteAttack(BossBase& boss) = 0;

};

class Context
{
    //--------------
    //-----攻撃-----
    //--------------

    // ①火の玉攻撃
    class FireBallAttack : public StrategyAttack
    {
        void ExecuteAttack(BossBase& boss) override;
    };

    // ②波状攻撃
    class WaveAttack : public StrategyAttack
    {
        void ExecuteAttack(BossBase& boss) override;
    };

    // ③弓矢攻撃
    class ArrowAttack : public StrategyAttack
    {
        void ExecuteAttack(BossBase& boss) override;
    };

    // ④突進攻撃
    class ChargeAttack : public StrategyAttack
    {
        void ExecuteAttack(BossBase& boss) override;
    };

    // ⑤斧投げ攻撃
    class AxeThrowAttack : public StrategyAttack
    {
        void ExecuteAttack(BossBase& boss) override;
    };

private:

    std::unique_ptr<StrategyAttack> currentAttack_; // 現在の攻撃戦略を保持するスマートポインタ

};