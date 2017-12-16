#ifndef SMART_LINKED_POINTER_H
#define SMART_LINKED_POINTER_H

#include <cassert>

template<typename T>
struct smart_shared_pointer{
    
    smart_shared_pointer() = default;

    explicit smart_shared_pointer(T* ptr) {
        pdata = new count_obj(ptr);
        pdata->cnt_refs = 1;
    }

    smart_shared_pointer(smart_shared_pointer const& other) noexcept : pdata(other.pdata) {
        if (!pdata) return ;
        (pdata->cnt_refs)++;
    }

    smart_shared_pointer(smart_shared_pointer&& other) noexcept
        : pdata(other.pdata) {
        other.pdata = nullptr;
    }

    smart_shared_pointer(std::nullptr_t) noexcept {}

    smart_shared_pointer& operator = (smart_shared_pointer const& other) noexcept {
        if (pdata == other.pdata) return *this;
        remove_node();
        pdata = other.pdata;
        if (!pdata) return *this;
        (pdata->cnt_refs)++;
        return *this;
    }

    smart_shared_pointer& operator = (smart_shared_pointer&& other) noexcept {
        remove_node();
        pdata = other.pdata;
        other.pdata = nullptr;
        return *this;
    }

    smart_shared_pointer& operator = (std::nullptr_t) noexcept {
        remove_node();
        pdata = nullptr;
        return *this;
    }

    ~smart_shared_pointer() noexcept {
        if (pdata) {
            --(pdata->cnt_refs);
            if (!pdata->cnt_refs) {
                delete pdata;
                pdata = nullptr;
            }
        }
    }

    T& operator *() const {
        return *pdata->data;
    }

    T* operator ->() const {
        return pdata->data;
    }

    friend void swap(smart_shared_pointer& p1, smart_shared_pointer& p2) noexcept {
        if (p1.pdata == p2.pdata) return ;
        std::swap(p1.pdata, p2.pdata);
    }

    operator bool() const noexcept{
        return pdata;
    }

    friend bool operator == (smart_shared_pointer const& a, smart_shared_pointer const& b) noexcept {
        return a.pdata == b.pdata;
    }

    friend bool operator != (smart_shared_pointer const& a, smart_shared_pointer const& b) noexcept {
        return !(a == b);
    }

    friend bool operator == (smart_shared_pointer const& a, std::nullptr_t) noexcept {
        return !a.pdata;
    }

    friend bool operator != (smart_shared_pointer const& a, std::nullptr_t) noexcept {
        return a.pdata;
    }

    friend bool operator == (std::nullptr_t, smart_shared_pointer const& a) noexcept {
        return a == nullptr;
    }

    friend bool operator != (std::nullptr_t, smart_shared_pointer const& a) noexcept {
        return a != nullptr;
    }
    
private:

    struct count_obj {
        int cnt_refs {};
        T* data {};

        count_obj() = default;

        count_obj(T *data) noexcept
            : data(data) {}

        ~count_obj() noexcept {
            delete data;
        }

    };

    count_obj *pdata {};

    void remove_node() noexcept {
        if (pdata) {
            --(pdata->cnt_refs);
            if (!pdata->cnt_refs) {
                delete pdata;
                pdata = nullptr;
            }
        }
    }

};
#endif // SMART_LINKED_POINTER_H
