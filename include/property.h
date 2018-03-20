#ifndef PROPERTY_H
#define PROPERTY_H

#include<functional>
#include<vector>

template<typename T> class property {
    typedef std::function<void(T)> listener;

    private:
        T val;
        std::vector<listener> listeners;
    public:
        property(T v) {
            this->val = v;
        }
        void set(T v) {
            this->val = v;
            for(listener& l : this->listeners)
                l(v);
        }
        T get() {
            return this->val;
        }
        void listen(listener l) {
            this->listeners.emplace_back(l);
        }
        void bind(property<T>& other) {
            other.listen([this](T newval){ this->set(newval); });
        }
};

#endif // PROPERTY_H
