// author:   C瓜哥
// e-mail:   tangcraig@gmail.com
// post-url: http://www.cguage.com/2013/08/obj-pool.html
// dsc:      对象池模板类

// main idea: 核心思路是预分配若个对象大小的空间，按照堆栈的方式
//            组件在一起，等待分配栈顶对象，释放时释放在栈顶
//            为了加速可以，每个对象多分配2个字节空间，使用Simulating
//            Pointer的方法，利用多于两个字节，填入下一元素索引值
//            使用类似FAT32的技术实现高速的分配和释放


#pragma once
#include <stack>
#include <assert.h>
#define Stack std::stack // 这里可替换成自行开发的高速栈

// a pool has a stack inside
// we can use std::stack
// or simulator pointer(FAT32) to implement a stack

template<class T, size_t max_size>
class ObjPool {
public:
    static ObjPool<T, max_size>& instance();
    T* alloc();
    void free(T* p);
    void freeAll();
    inline size_t count() {return max_size;}

protected:
    ObjPool();
    ~ObjPool();

private:
    T* m_objects;
    Stack<T*> m_FreeObjects;
};

#define USING_DIRTY_DATA true
// 如果不是方便测试需要，可以将这行
// typedef ObjPool<obj_class, max_size> obj_class##Pool; \
// 标注为private
#define DECLARE_USING_OBJ_POOL(obj_class, max_size, _using_dirty_data) \
    public: \
        typedef ObjPool<obj_class, max_size> obj_class##Pool; \
	    friend class obj_class##Pool; \
        static const bool using_dirty_data = _using_dirty_data; \
    public: \
    ~obj_class() { \
        if (!_using_dirty_data) {this->purge();} \
    } \
    static void* operator new(size_t) { \
        return obj_class##Pool::instance().alloc(); \
    } \
    static void operator delete(void* p) { \
        obj_class##Pool::instance().free(reinterpret_cast<obj_class*>(p)); \
    } \
    static bool loadCache() { \
        while (true) { \
            obj_class* obj = new obj_class; \
            if (obj != NULL) { \
                if (!obj->init()) { \
                    return false; \
                } \
            } else { \
                break; \
            } \
        }; \
        obj_class##Pool::instance().freeAll(); \
        return true; \
    }


//
// IMPLEMENTS
//

template<class T, size_t max_size>
ObjPool<T, max_size>& ObjPool<T, max_size>::instance() {
    static ObjPool<T, max_size> pool;
    return pool;
}

template<class T, size_t max_size>
ObjPool<T, max_size>::ObjPool() :
  m_objects((T*)new char[max_size * sizeof(T)]) {
    memset(m_objects, 0, max_size * sizeof(T));
    freeAll();
}

template<class T, size_t max_size>
ObjPool<T, max_size>::~ObjPool() {
    // 如果有必要，清空附带资源。
    if (T::using_dirty_data) {
        for (int i = 0; i < max_size; ++i) {
            T* p = &m_objects[i];
            p->purge();
        }
    }
    // 释放对象所占空间
    delete[] (char*)m_objects;
    m_objects = NULL;
}

template<class T, size_t max_size>
T* ObjPool<T, max_size>::alloc() {
    if (!m_FreeObjects.empty()) {
        T* obj = m_FreeObjects.top();
        m_FreeObjects.pop();
        return obj;
    }
    return NULL;
}

template<class T, size_t max_size>
void ObjPool<T, max_size>::free(T* p) {
    assert(p != NULL);
    // TODO: debug版需要检查p是否在对象池中
    // ...
    m_FreeObjects.push(p);
}

template<class T, size_t max_size>
void ObjPool<T, max_size>::freeAll() {
    for (size_t i = 0; i < max_size; ++i) {
        m_FreeObjects.push(&m_objects[i]);
    }
}