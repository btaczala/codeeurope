#pragma once

template <class T>
struct remove_pointer {
    typedef T type;
};
template <class T>
struct remove_pointer<T *> {
    typedef T type;
};
template <class T>
struct remove_pointer<T *const> {
    typedef T type;
};
template <class T>
struct remove_pointer<T *volatile> {
    typedef T type;
};
template <class T>
struct remove_pointer<T *const volatile> {
    typedef T type;
};

std::string freeFunction(const std::string &,
                         const std::vector<std::string> &vec) {
    return "";
}

struct SomeType {
    std::string aFunction(const std::string &,
                          const std::vector<std::string> &vec) {
        return "";
    }
};
