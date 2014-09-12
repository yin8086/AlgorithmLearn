// author:   C瓜哥
// e-mail:   tangcraig@gmail.com
// post-url: http://www.cguage.com/2013/08/obj-pool.html
// dsc:      游戏开发中的一种内存管理模型：对象池

#include <iostream>
#include <list>
#include <time.h>
#include "ObjPool.h"
#include "ObjManager.h"
using namespace std;

#define NOT !
#define NO  !
#define IS_VALID_POINTER(p) (p != NULL)
#define log cout<<"["<<get_elapsed_time()<<"sec] "
const int BULLET_POOL_VOLUM = 3;

time_t get_elapsed_time() {
    static time_t prev = time(NULL);
    return time(NULL) - prev;
}

class Sprite {
public:
    Sprite(size_t buf_size) : 
      m_buf(new char[buf_size]),
      m_bufSize(buf_size) {
    }
    ~Sprite() {
        log << "freeing sprite buf. size:" 
             << m_bufSize << endl;
        delete[] m_buf;
        m_buf = NULL;
    }

private:
    char* m_buf;
    size_t m_bufSize;
};

class Entity {
public:
    Entity() : 
     m_id(++s_counter),
     m_hp(-1),
     m_valid(true) {
    }
    virtual ~Entity() {}
    virtual bool init() = 0;
    virtual Entity* clone() = 0;
    bool isValid() {
        return m_valid;
    }
    void update() {
        assert(m_valid && "updating invalid entity!");
        --m_hp;
        if (NO m_hp) {
            // 为方便外部引用本对象的指针得到更新，一般不使用以下方法
            // delete this;
            // 当然在类中delete也可以通知到外部引用，这就要用到shared_ptr, weak_ptr了
            // 方便起见，还是如下使用：
            m_valid = false;
        }
    }
    inline void destroy() {
        log << "destroy entity" << m_id << endl;
        delete this;
    }

protected:
    static int s_counter;
    int m_id;
    int m_hp;
    bool m_valid;
    double m_JustEnlargeSpace[200];
};
int Entity::s_counter = 0;

class Bullet : public Entity {
    // factory for generating Bullet which have interface Entity
    DECLARE_OBJ_FACTORY(Entity)
    // DECLARE_USING_OBJ_POOL(Bullet, BULLET_POOL_VOLUM, (NOT USING_DIRTY_DATA))
    DECLARE_USING_OBJ_POOL(Bullet, BULLET_POOL_VOLUM, USING_DIRTY_DATA)

public:
    Bullet() : m_b(1) {
        m_hp = 1 + rand() % 5;
        log << "Bullet"  << m_id 
             << " with HP:" << m_hp << endl;

        // #if (NOT using_dirty_data)
        if (NOT using_dirty_data) {
            init();
        }
    }
    Bullet(const Bullet* that) {
        this->m_id = ++s_counter;
        this->m_b = that->m_b;
        this->m_hp = that->m_hp;
        if (NOT using_dirty_data) {
            // TODO: 不使用脏数据就需要拷贝Sprite
        }
    }
    Entity* clone() {
        return new Bullet(this);
    }

protected:
    bool init() {
        m_texture = new Sprite(m_id);
        log << "init Bullet" << m_id << endl;
        return true;
    }
    void purge() {
        log << "purge Bullet" << m_id << endl;
        delete m_texture;
        m_texture = NULL;
    }

private:
    int m_b;
    Sprite* m_texture;
};
IMPL_OBJ_FACTORY(Entity, Bullet);

int main(int argc, char* argv[]) {
    // 为了方便监视对象池分配情况，这里可以把对象池拿出来
    Bullet::BulletPool& pool = Bullet::BulletPool::instance();
    
    // 加载，把慢的操作比如读取图片放这里。这里会初始
    // 化整个池中的对象，具体见源码
    if (Bullet::using_dirty_data) {
        log << "加载缓存" << endl;
        Bullet::loadCache();
        log << endl;
    }
    // 测试实体工厂能否产出子弹
    Bullet* bullet = dynamic_cast<Bullet*>(
        ObjManager<Entity>::instance().make("Bullet")
    );
    assert(bullet != NULL);

    // 引入智能指针，测试兼容性
    weak_ptr<Entity> entity_ref;
    {    
        Entity* raw_p = ObjManager<Entity>::instance().make("Bullet");
        shared_ptr<Entity> entity(raw_p);
        entity_ref = entity;
    }
    shared_ptr<Entity> the_entity = entity_ref.lock();
    assert(the_entity.get() == NULL);

    // 删除之前分配的子弹，这时对象池应该处于全未分配状态 
    delete bullet;

    // 测试超额分配
    Entity* p1 = ObjManager<Entity>::instance().make("Bullet");
    Entity* p2 = ObjManager<Entity>::instance().make("Bullet");
    Entity* p3 = ObjManager<Entity>::instance().make("Bullet");
    Entity* p4 = ObjManager<Entity>::instance().make("Bullet");
    assert(p4 == NULL);
    p1->destroy();
    p4 = ObjManager<Entity>::instance().make("Bullet");
    assert(p1 == p4);
    p2->destroy();
    p3->destroy();
    p4->destroy();

    // 大规模测试
    list<Entity*> timer;
    struct _Timer{
        list<Entity*>& _timer;
        _Timer(list<Entity*>& timer) : _timer(timer) {}

        void operator()() {
            for (list<Entity*>::iterator iter = _timer.begin();
                iter != _timer.end();) {
                Entity* entity = *iter;
                if (entity->isValid()) {
                    (*iter)->update();
                } else {
                    entity->destroy();
                    iter = _timer.erase(iter);
                    continue;
                }
                ++iter;
            } // end for
        }
    } update_timer(timer);

    const int num = 50;
    log << endl << "大规模测试:" << endl;
    for (int i = 0; i < num; ++i) {
        Entity* entity = ObjManager<Entity>::instance().make("Bullet");
        if (IS_VALID_POINTER(entity)) {
            log << "  alloced index:" << i << endl;
            timer.push_back(entity);
        } else {
            log << "  alloc bullet failed, waiting..." << endl;
            // 失败了就多尝试一次，反正任务量是20个
            --i;
        }
        update_timer();
    }

    // 不管使用什么模式都要让池回收所有的对象，
    // 不要依赖于池析构时的对象释放
    for (list<Entity*>::iterator iter = timer.begin();
        iter != timer.end(); ++iter) {
        (*iter)->destroy();
    }

    log << endl << "释放池" << endl;

    return 0;
}
