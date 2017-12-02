#ifndef SMART_LINKED_POINTER_H
#define SMART_LINKED_POINTER_H

template<typename T>
struct smart_shared_pointer{
    
    using value_type = T;
   
    smart_shared_pointer() = default;

    explicit smart_shared_pointer(T *nptr) : ptr(nptr) {
        count = new count {0};
    }
    
    
    
    
private:
    int* count {};
    T* ptr {};

}
#endif // SMART_LINKED_POINTER_H
