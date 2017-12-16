#ifndef SMART_SHARED_POINTER_H
#define SMART_SHARED_POINTER_H

template <typename T>
struct smart_linked_pointer {

    smart_linked_pointer() = default;

    explicit smart_linked_pointer(T* ptr) noexcept {
        pdata = ptr;
    }

    smart_linked_pointer(smart_linked_pointer const& other) noexcept : pdata(other.pdata) {
        if (pdata == nullptr) return ;
        left = &other;
        right = other.right;
        if (other.right) {
            other.right->left = this;
        }
        other.right = this;
    }

    smart_linked_pointer(smart_linked_pointer&& other) noexcept
        : pdata(other.pdata), left(other.left), right(other.right) {
        other.pdata = nullptr;
        other.left = other.right = nullptr;
        if (left) left->right = this;
        if (right) right->left = this;
    }

    smart_linked_pointer(std::nullptr_t) noexcept {}

    smart_linked_pointer& operator = (smart_linked_pointer const& other) noexcept {
        if (pdata == other.pdata) return ;
        remove_node();
        pdata = other.pdata;
        if (pdata == nullptr) return ;
        left = &other;
        right = other.right;
        if (other.right) {
            other.right->left = this;
        }
        other.right = this;
        return *this;
    }

    smart_linked_pointer& operator = (smart_linked_pointer&& other) noexcept {
        remove_node();
        pdata = other.pdata;
        left = other.left;
        right = other.right;
        other.pdata = nullptr;
        other.left = other.right = nullptr;
        if (left) left->right = this;
        if (right) right->left = this;

        return *this;
    }

    smart_linked_pointer& operator = (std::nullptr_t) noexcept {
        remove_node();
        return *this;
    }

    ~smart_linked_pointer() noexcept {
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

    friend void swap(smart_linked_pointer& p1, smart_linked_pointer& p2) noexcept {
        if (p1.pdata == p2.pdata) return ;
        std::swap(p1.pdata, p2.pdata);
        std::swap(p1.left, p2.left);
        std::swap(p1.right, p2.right);
    }

    operator bool() const noexcept {
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

    friend bool operator == (std::nullptr_t, smart_linked_pointer const& a) noexcept {
        return a == nullptr;
    }

    friend bool operator != (std::nullptr_t, smart_linked_pointer const& a) noexcept {
        return a != nullptr;
    }

private:

    T *pdata {nullptr};
    mutable smart_linked_pointer *left {nullptr};
    mutable smart_linked_pointer *right {nullptr};

    void remove_node() noexcept {
        if (left) left->right = right;
        if (right) right->left = left;
        if (!left && !right) {
            delete pdata;
        }
        left = right = nullptr;
        pdata = nullptr;
    }
};

#endif // SMART_SHARED_POINTER_H

