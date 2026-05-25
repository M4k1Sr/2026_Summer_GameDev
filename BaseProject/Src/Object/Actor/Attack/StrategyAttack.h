#pragma once
#include <memory>

class BossBase;

// ==========================================
// 攻撃パターンの基底クラス（共通の看板）
// ==========================================
class StrategyAttack
{
public:
    virtual ~StrategyAttack() = default;

    // 実行関数
    virtual void ExecuteAttack(BossBase& boss) = 0;
}; // ここで一度、看板クラスのカッコを「閉じます」！


// ==========================================
// 具象クラス（それぞれの攻撃は外側に独立して並べる）
// ==========================================

// ①火の玉攻撃
class FireBallAttack : public StrategyAttack
{
public: // 外部から呼び出せるように public をつけます
    void ExecuteAttack(BossBase& boss) override;
};

// ②波状攻撃
class WaveAttack : public StrategyAttack
{
public:
    void ExecuteAttack(BossBase& boss) override;
};

// ③弓矢攻撃
class ArrowAttack : public StrategyAttack
{
public:
    void ExecuteAttack(BossBase& boss) override;
};

// ④突進攻撃
class ChargeAttack : public StrategyAttack
{
public:
    void ExecuteAttack(BossBase& boss) override;
};

// ⑤斧投げ攻撃
class AxeThrowAttack : public StrategyAttack
{
public:
    void ExecuteAttack(BossBase& boss) override;
};