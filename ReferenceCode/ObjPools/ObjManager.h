// author:   C瓜哥
// e-mail:   tangcraig@gmail.com
// post-url: http://www.cguage.com/2013/08/obj-pool.html
// dsc:      对象工厂类，使用宏和预加载提高效率

#pragma once
#include <string>
#include <map>

template<class T>
class ObjManager {
    typedef T* (*Creator)(void);
    typedef std::map<std::string, Creator> ClassMap;

public:
    static ObjManager<T>& instance();
    bool registerClass(const char* class_name, Creator creator);
    T* make(const char* class_name);

private:
    ClassMap m_ClassMap;
};

template<typename T>
class DynamicClass {
public:
    DynamicClass(const char* class_name, T* (*creator)(void)) {
        ObjManager<T>::instance().registerClass(class_name, creator);
    }
};

#define DECLARE_OBJ_FACTORY(base_class) \
    private: static DynamicClass<base_class> m_ClassInfo;

#define IMPL_OBJ_FACTORY(base_class, sub_class) \
    static base_class* _creator() { \
        return new sub_class; \
    } \
    DynamicClass<base_class> sub_class::m_ClassInfo(#sub_class, _creator);

//
// IMPLEMENTS
//

template<class T>
ObjManager<T>& ObjManager<T>::instance() {
    static ObjManager<T> instance;
    return instance;
}

template<class T>
bool ObjManager<T>::registerClass(const char* class_name, Creator creator) {
    return m_ClassMap.insert(std::make_pair(std::string(class_name), creator)).second;
}

template<class T>
T* ObjManager<T>::make(const char* class_name) {
    ClassMap::iterator itor = m_ClassMap.find(std::string(class_name));
    if (itor != m_ClassMap.end()) {
        return (itor->second)();
    }
    return NULL;
}
