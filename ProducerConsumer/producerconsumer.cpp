#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <list>
#include <functional>
#include <unistd.h>
template<typename T>
class SyncQueue {
public:
    explicit SyncQueue(size_t size) : max_size(size) {}

    void Put(const T &t) {
        std::unique_lock<std::mutex> lock(mtx_);
        while(list_.size() == max_size ) { // 这里不要调用Full(),因为在full中又有上锁
            std::cout << "任务队列已满，producer等待。。。" << std::endl;
            // 等待队列不满
            not_full_.wait(lock);
        }
        list_.push_back(t);
        std::cout << t << "被生产。。。" << std::endl;
        // 通知消费者队列不空
        not_empty_.notify_one();
    }
    // wait的另一种方式
    void put(const T &t) {
        std::unique_lock<std::mutex> lock(mtx_);
        // 根据条件阻塞线程,满足条件不阻塞
        not_full_.wait(lock,[this]() {
            return Full();
        });
        
        list_.push_back(t);
        std::cout << t << "被生产。。。" << std::endl;
        // 通知消费者队列不空
        not_empty_.notify_one();
    }
    T Take() {
        std::unique_lock<std::mutex> lock(mtx_);
        while(list_.empty()) {
            std::cout << "任务队列空,consumer等待。。。" << std::endl;
            // 等待队列不空
            not_empty_.wait(lock);
        }
        T t = list_.front();
        list_.pop_front();
        // 通知生产者队列不满，可以生产
        not_full_.notify_one();
        std::cout << t << " 被消费" << std::endl;
        return t;

    }

    T take() {
        std::unique_lock<std::mutex> lock(mtx_);
        not_empty_.wait(lock, [this]() {
            return Empty();
        });
        T t = list_.front();
        list_.pop_front();
        not_full_.notify_one();
        std::cout << t << " 被消费" << std::endl;
        return t;

    }
    bool Empty() noexcept { 
        std::lock_guard<std::mutex> lock(mtx_);
        return list_.empty(); 
    }
    bool Full() noexcept { 
        std::lock_guard<std::mutex> lock(mtx_);
        return list_.size() == max_size; 
    }
    size_t size() noexcept { 
        std::lock_guard<std::mutex> lock(mtx_);
        return list_.size(); 
    }
private:
    std::list<T> list_;
    std::mutex mtx_;
    std::condition_variable not_empty_;
    std::condition_variable not_full_;
    size_t max_size;
};

int main(int argc, char *argv[]) {
    SyncQueue<int> task_queue(50);
    auto producerF = std::bind(&SyncQueue<int>::Put, &task_queue, std::placeholders::_1);
    auto consumerF = std::bind(&SyncQueue<int>::Take, &task_queue);

    std::thread producers[3];
    std::thread consumers[3];

    for(int i = 0; i < 3; i++) {
        producers[i] = std::thread(producerF, i + 100);
        consumers[i] = std::thread(consumerF);
    }
    
    for(int i = 0; i < 3; i++) {
        producers[i].join();
        consumers[i].join();
    }

    return 0;
}