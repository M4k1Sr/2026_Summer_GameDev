#pragma once
#include <unordered_map>
#include <typeindex>

enum class Lifecycle { Transient, Singleton };

class DIContainer {
private:

    // 型ごとにインスタンスを保持
    std::unordered_map<std::type_index, void*> singletons_;     // シングルトンインスタンス
	std::unordered_map<std::type_index, Lifecycle> lifecycles_; // 登録時のライフサイクル

public:

    // 登録
    template<typename T>
    void Register(Lifecycle life = Lifecycle::Transient) {
        // ライフサイクルを保存
        lifecycles_[typeid(T)] = life;
        // シングルトンの場合インスタンスを生成して保存
        if (life == Lifecycle::Singleton) {
            singletons_[typeid(T)] = new T();
        }
    }

    // 取り出し
    template<typename T>
    T* Resolve() {
        // 登録されているライフサイクルの確認
        auto it = lifecycles_.find(typeid(T));
        // 登録されていなかった場合はnullを返す
        if (it == lifecycles_.end()) {
            return nullptr;
        }
        // シングルトンの場合は保存されているインスタンスを返す
        if (it->second == Lifecycle::Singleton) {
            return static_cast<T*>(singletons_[typeid(T)]);
        }
        return new T();
    }

    // デストラクタで全部解放
    ~DIContainer() {
        for (auto& pair : singletons_) {
            // 解放はマネージャのReleaseを呼んでから
        }
    }
};