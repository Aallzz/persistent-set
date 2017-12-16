#ifndef PERSISTENT_SET_H
#define PERSISTENT_SET_H

#include <memory>
#include <vector>
#include <cassert>
#include "smart_shared_pointer.h"

template <typename T, template<typename> class scoped_ptr = smart_shared_pointer>
struct persistent_set
{
    using value_type = T;

    struct iterator;

    persistent_set() {}

    persistent_set(persistent_set const& other) noexcept {
        root = other.root;
    }

    persistent_set(persistent_set&& other) noexcept {
        root = other.root;
        other.root = nullptr;
    }

    ~persistent_set() noexcept {}

    persistent_set& operator=(persistent_set const& other) noexcept {
        root = other.root;
        return *this;
    }

    persistent_set& operator=(persistent_set&& other) noexcept {
        root = other.root;
        other.root = nullptr;
        return *this;
    }


    iterator find(value_type value) {
        return iterator(root, get(root, value));
    }

    std::pair<iterator, bool> insert(value_type const& value) {
        if (get(root, value)) return {iterator(root, get(root, value)), false};
        root = put(root, value);
        return {iterator(root, get(root, value)), true};
    }

    std::pair<iterator, bool> insert(value_type&& value) {
        if (get(root, value)) return {iterator(root, get(root, value)), false};
        root = put(root, value);
        return {iterator(root, get(root, value)), true};
    }

    void erase(iterator it) {
        root = del(root, *it);
    }

    iterator begin() const {
        return iterator(root, getMin(root));
    }

    iterator end() const {
        return iterator(root, nullptr);
    }

private:

    struct node {
        T key {};
        scoped_ptr<node> left {nullptr};
        scoped_ptr<node> right {nullptr};

        explicit node(T const& val, scoped_ptr<node> left = nullptr, scoped_ptr<node> right = nullptr)
            : key(val), left(left), right(right) {}

        explicit node(T&& val, scoped_ptr<node> left = nullptr, scoped_ptr<node> right = nullptr)
            : key(std::move(val)), left(left), right(right) {}
    };

    scoped_ptr<node> root {nullptr};
    scoped_ptr<node> prev_root {nullptr};

    scoped_ptr<node> get(scoped_ptr<node> cur, T k) const {
        if (cur == nullptr) return nullptr;
        if (k < cur->key)
            return get(cur->left, k);
        else if (k > cur->key)
            return get(cur->right, k);
        else return cur;
    }

    scoped_ptr<node> put(scoped_ptr<node> cur, T k) {
        if (cur == nullptr) {
            return scoped_ptr<node>(new node(k));
        }

        scoped_ptr<node> new_cur = scoped_ptr<node>(new node(cur->key, cur->left, cur->right));

        if (k < cur->key) {
            new_cur->left = put(cur->left, k);
        } else if (k > cur->key) {
            new_cur->right = put(cur->right, k);
        }

        return new_cur;
    }

    scoped_ptr<node> put(scoped_ptr<node> cur, T&& k) {
        if (cur == nullptr) {
            return scoped_ptr<node>(new node(k));
        }

        scoped_ptr<node> new_cur = scoped_ptr<node>(cur);

        if (k < cur->key) {
            new_cur->left = put(cur->left, k);
        } else if (k > cur->key) {
            new_cur->right = put(cur->right, k);
        }

        return new_cur;
    }

    friend scoped_ptr<node> getMin(scoped_ptr<node> cur) noexcept {
        if (!cur) return nullptr;
        if (cur->left == nullptr)
            return cur;
        else
            return getMin(cur->left);
    }

    friend scoped_ptr<node> getMax(scoped_ptr<node> cur) noexcept {
        if (!cur) return nullptr;
        if (cur->right == nullptr)
            return cur;
        else
            return getMax(cur->right);
    }

    scoped_ptr<node> delMin(scoped_ptr<node> cur) {
        if (!cur) return nullptr;
        if (cur->left == nullptr)
            return cur->right ? scoped_ptr<node>(new node(cur->right->key, cur->right->left, cur->right->right)) : nullptr;
        scoped_ptr<node> new_cur = scoped_ptr<node>(new node(cur->key, cur->left, cur->right));
        new_cur->left = delMin(cur->left);
        return new_cur;
    }

    scoped_ptr<node> del(scoped_ptr<node> cur, T k) {
        if (cur == nullptr) return nullptr;

        scoped_ptr<node> new_cur = scoped_ptr<node>(new node(cur->key, cur->left, cur->right));

        if (k < cur->key) {
            new_cur->left = del(cur->left, k);
        } else if (k > cur->key) {
            new_cur->right = del(cur->right, k);
        } else {
            if (cur->right == nullptr) {
                if (!cur->left) return nullptr;
                return scoped_ptr<node>(new node(cur->left->key, cur->left->left, cur->left->right));
            }
            if (cur->left == nullptr) {
                if (!cur->right) return nullptr;
                return scoped_ptr<node>(new node(cur->right->key, cur->right->left, cur->right->right));
            }
            scoped_ptr<node> tmp = scoped_ptr<node>(new node(cur->key, cur->left, cur->right));
            scoped_ptr<node> ttmp = getMin(cur->right);

            new_cur = scoped_ptr<node>(new node(ttmp->key, ttmp->left, ttmp->right));

            new_cur->right  = delMin(tmp->right);
            new_cur->left = tmp->left;
        }

        return new_cur;
    }

    friend scoped_ptr<node> prev(scoped_ptr<node> cur, T k) {
        if (cur == nullptr) return nullptr;
        std::vector<scoped_ptr<node>> path;
        path.push_back(nullptr);
        while (cur && cur->key != k) {
            path.push_back(cur);
            if (cur->key > k) {
                cur = cur->left;
            } else if (cur->key < k) {
                cur = cur->right;
            }
        }
        assert(cur != nullptr);
        if (cur->left) return getMax(cur->left);
        while (!path.empty()) {
            if (path.back()->right == cur) return path.back();
            cur = path.back();
            path.pop_back();
        }
        return nullptr;
    }

    friend scoped_ptr<node> next(scoped_ptr<node> cur, T k) {
        if (cur == nullptr) return nullptr;
        std::vector<scoped_ptr<node>> path;
        path.push_back(nullptr);
        while (cur && cur->key != k) {
            path.push_back(cur);
            if (cur->key > k) {
                cur = cur->left;
            } else if (cur->key < k) {
                cur = cur->right;
            }
        }
        assert(cur != nullptr);
        if (cur->right) return getMin(cur->right);
        while (!path.empty()) {
            if (path.back()->left == cur) return path.back();
            cur = path.back();
            path.pop_back();
        }
        return nullptr;
    }
};

template <typename T, template<typename> class scoped_ptr>
struct persistent_set<T, scoped_ptr>::iterator
{

    value_type const& operator*() const {
        return ptr->key;
    }

    iterator& operator++() {
        if (getMax(owner) == ptr)
            ptr = nullptr;
        else
            ptr = next(owner, ptr->key);
        return *this;
    }

    iterator operator++(int) {
        iterator i = *this;
        ++(*this);
        return i;
    }

    iterator& operator--() {
        if (ptr == nullptr) ptr = getMax(owner);
        else ptr = prev(owner, ptr->key);
        return *this;
    }

    iterator operator--(int) {
        iterator i = *this;
        --(*this);
        return i;
    }

    iterator(scoped_ptr<node> const& owner, scoped_ptr<node> const& ptr) noexcept
        : ptr(ptr), owner(owner) {
    }

    iterator(scoped_ptr<node>&& owner, scoped_ptr<node>&& ptr) noexcept
        : ptr(ptr), owner(owner) {
    }

    friend bool operator ==(iterator const& a, iterator const& b) noexcept {
        return (a.owner == b.owner && a.ptr == b.ptr);
    }

    friend bool operator !=(iterator const& a, iterator const& b) noexcept {
        return !(a == b);
    }

    ~iterator() {

    }

private:

    scoped_ptr<node> ptr;
    scoped_ptr<node> owner;
};

#endif // PERSISTENT_SET_H

