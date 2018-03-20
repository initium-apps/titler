#ifndef UTILS_H
#define UTILS_H

#include<algorithm>
#include<vector>

template<typename T>
T clip(const T& what, const T& lo, const T& hi) {
    return std::max(lo, std::min(hi, what));
}

template<typename T>
void removeFromVector(std::vector<T>& v, T val) {
    size_t j = 0;
    for(size_t i = 0; i < v.size(); ++i) {
        if (v[i] != val)
            v[j++] = v[i];
    }
    v.resize(j);
}

#endif // UTILS_H
