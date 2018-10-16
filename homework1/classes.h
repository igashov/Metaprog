#ifndef task1_classes_h
#define task1_classes_h

#include <type_traits>

typedef int ProxyType;
typedef float MediatorType;
typedef int* ObserverType;

template<class T, class Enable = void>
class C;

// Observer
template<class T>
class C<T, typename std::enable_if<std::is_pointer<T>::value >::type> {
public:
    void notify(int value) {
        std::cout << "  Observer notification: value changed to " << value << std::endl;
    }
};

class B {
private:
    int value;
    C<ObserverType>* observer = nullptr;
    
public:
    void become_observer(C<ObserverType>* obs) {
        observer = obs;
    }
    
    void set_value(int v) {
        std::cout << "      B: set value " << v << std::endl;
        value = v;
        if (observer != nullptr) {
            observer->notify(value);
        }
    }
    
    int get_value() {
        std::cout << "      B: get value" << std::endl;
        return value;
    }
};

// Proxy
template<class T>
class C<T, typename std::enable_if<std::is_integral<T>::value >::type> {
private:
    B b;
public:
    void set_observer(C<ObserverType>* observer) {
        b.become_observer(observer);
    }
    
    void set_value(int value) {
        std::cout << "  Proxy: set value " << value << std::endl;
        b.set_value(value);
    }
    
    int get_value() {
        std::cout << "  Proxy: get value" << std::endl;
        int value = b.get_value();
        return value;
    }
};

// Mediator
template<class T>
class C<T, typename std::enable_if<std::is_floating_point<T>::value >::type> {
private:
    B array[5];
    int current = 0;
    
public:
    void set_value(int value) {
        std::cout << "  Mediator: set value " << value << std::endl;
        array[current].set_value(value);
        current = (current + 1) % 5;
    }
    
    void show_values() {
        std::cout << "  Mediator: show values" << std::endl;
        for (int i = 0; i < 5; ++i) {
            int value = array[i].get_value();
            std::cout << "  " << value << std::endl;
        }
    }
};

class A {
private:
    C<ProxyType> proxy;
    C<MediatorType> mediator;
    C<ObserverType> observer;
    
public:
    // Proxy method
    void check_proxy() {
        int value_to_set = 10;
        std::cout << "Start checking Proxy with value " << value_to_set << std::endl;
        proxy.set_observer(&observer);
        proxy.set_value(value_to_set);
        int value_got = proxy.get_value();
        std::cout << "End checking Proxy. Got value " << value_got << std::endl;
    }
    
    // Mediator method
    void check_mediator(int values_to_set[5]) {
        std::cout << "Start checking Mediator with values ";
        for (int i = 0; i < 5; ++i) {
            std::cout << values_to_set[i] << ' ';
        }
        std::cout << std::endl;
        
        for (int i = 0; i < 5; ++i) {
            mediator.set_value(values_to_set[i]);
            mediator.show_values();
        }
        std::cout << "End checking Mediator" << std::endl;
    }    
};

#endif
