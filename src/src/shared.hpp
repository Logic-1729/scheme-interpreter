#ifndef SHARED_PTR
#define SHARED_PTR

struct pt_cnt {
    int shared_cnt = 1;
    int weak_cnt = 0;
};

template <typename T>
class SharedPtr {
public:
    T* _p;
    pt_cnt* num;

    void clear() {
        if (_p && (--num->shared_cnt == 0)) {
            delete _p;
            if (num->weak_cnt == 0) {
                delete num;
            }
        }
        _p = nullptr;
        num = nullptr;
    }

    SharedPtr() : _p(nullptr), num(nullptr) {}

    explicit SharedPtr(T* p) : _p(p), num(p ? new pt_cnt() : nullptr) {}

    SharedPtr(const SharedPtr& temp) : _p(temp._p), num(temp.num) {
        if (num) {
            num->shared_cnt += 1;
        }
    }

    ~SharedPtr() { clear(); }

    int use_count() const { return num ? num->shared_cnt : 0; }

    T* get() const { return _p; }

    operator bool() const { return _p != nullptr; }

    T& operator*() const { return *_p; }

    T* operator->() const { return _p; }

    SharedPtr& operator=(const SharedPtr& temp) {
        if (this != &temp) {
            auto new_p = temp._p;
            auto new_num = temp.num;
            if (new_num) {
                new_num->shared_cnt += 1;
            }
            clear();
            _p = new_p;
            num = new_num;
        }
        return *this;
    }

    void reset() { clear(); }

    void reset(T* p) {
        clear();
        if (p) {
            _p = p;
            num = new pt_cnt();
        }
    }
};

template <typename T, typename... Ts>
SharedPtr<T> make_shared(Ts&&... args) {
    return SharedPtr<T>(new T(std::forward<Ts>(args)...));
}
#endif
