#ifndef SMART_SHARED_POINTER_H
#define SMART_SHARED_POINTER_H

template <typename T>
struct smart_linked_pointer {

    smart_linked_pointer() = default;

    explicit smart_linked_pointer(T* ptr) {
        pdata = ptr;
    }

    smart_linked_pointer(smart_linked_pointer const& other) : pdata(other.pdata) {
        if (pdata == nullptr) return ;
        left = &other;
        right = other.right;
        if (other.right) {
            other.right->left = this;
        }
        other.right = this;
    }

    smart_linked_pointer(smart_linked_pointer&& other)
        : pdata(other.pdata), left(other.left), right(other.right) {
        other.pdata = nullptr;
        other.left = other.right = nullptr;
        if (left) left->right = this;
        if (right) right->left = this;
    }

    smart_linked_pointer(std::nullptr_t) {}

    smart_linked_pointer& operator = (smart_linked_pointer const& other) {
        if (pdata == other.pdata) return ;
        if (left) left->right = right;
        if (right) right->left = left;
        if (!left && !right)
            delete pdata, pdata = nullptr;
        pdata = other.pdata;
        left = right = nullptr;
        if (pdata == nullptr) return ;
        left = &other;
        right = other.right;
        if (other.right) {
            other.right->left = this;
        }
        other.right = this;
        return *this;
    }

    smart_linked_pointer& operator = (smart_linked_pointer&& other) {
        if (left) left->right = right;
        if (right) right->left = left;
        if (!left && !right) {
            delete pdata;
        }
        pdata = other.pdata;
        left = other.left;
        right = other.right;
        other.pdata = nullptr;
        other.left = other.right = nullptr;
        if (left) left->right = this;
        if (right) right->left = this;

        return *this;
    }

    smart_linked_pointer& operator = (std::nullptr_t) {
        if (left) left->right = right;
        if (right) right->left = left;
        if (!left && !right) {
            delete pdata;
        }
        left = right = nullptr;
        pdata = nullptr;
        return *this;
    }

    ~smart_linked_pointer() {
        if (right) right->left = left;
        if (left) left->right = right;
        if (!left && !right)
            delete pdata, pdata = nullptr;
    }

    T& operator *() const {
        return *pdata;
    }

    T* operator ->() const {
        return pdata;
    }

//    friend void swap(smart_linked_pointer& p1, smart_linked_pointer& p2) {
//        if (p1.pdata == p2.pdata) return ;
//        std::swap(p1.pdata, p2.pdata);
//        std::swap(p1.left, p2.left);
//        std::swap(p1.right, p2.right);
//    }

    operator bool() const{
        return pdata;
    }

    friend bool operator == (smart_linked_pointer const& a, smart_linked_pointer const& b) noexcept {
        return a.pdata == b.pdata;
    }

    friend bool operator != (smart_linked_pointer const& a, smart_linked_pointer const& b) noexcept {
        return !(a == b);
    }

    friend bool operator == (smart_linked_pointer const& a, std::nullptr_t) noexcept {
        return !a.pdata;
    }

    friend bool operator != (smart_linked_pointer const& a, std::nullptr_t) noexcept {
        return a.pdata;
    }

private:

    T *pdata {nullptr};
    mutable smart_linked_pointer *left {nullptr};
    mutable smart_linked_pointer *right {nullptr};
};

#endif // SMART_SHARED_POINTER_H

