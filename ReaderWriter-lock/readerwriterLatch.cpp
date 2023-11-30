#pragma once

#include <climits>
#include <condition_variable>
#include <mutex>

class ReaderWriterLatch {
    using mutex_t = std::mutex;
    using conn_t = std::condition_variable;
    static const uint32_t MAX_READER = UINT_MAX;
public:
    ReaderWriterLatch() = default;
    ~ReaderWriterLatch() {
        // 在析构之前再次获取锁，确保没有更多工作正在进行中
        std::unique_lock<mutex_t> latch(mtx_);
    }

    ReaderWriterLatch(const ReaderWriterLatch &rhs) = delete;
    ReaderWriterLatch &operator=(const ReaderWriterLatch &) = delete;

    void WLock();
    void WUnlock();
    void RLock();
    void RUnlock(); 
private:
    mutex_t mtx_;
    conn_t reader_;
    conn_t writer_;
    uint32_t reader_count_{0};
    bool writer_entered_{false};
};

void ReaderWriterLatch::WLock() {
    std::unique_lock<mutex_t> latch(mtx_);
    while (writer_entered_) {
        // 同一时刻只能有一个writer写
        reader_.wait(latch);
    }
    writer_entered_ = true;
    // 设置为true，不再允许reader读
    while (reader_count_ > 0) {
        // 先让所有的读者完成退出
        writer_.wait(latch);
    }
}
void ReaderWriterLatch::WUnlock() {
    std::unique_lock<mutex_t> latch(mtx_);
    // 表明没有更多的writer正在进行中
    writer_entered_ = false;
    // 通知新的reader和writer可以继续了
    reader_.notify_all();
}
void ReaderWriterLatch::RLock() {
    std::unique_lock<mutex_t> latch(mtx_);
    // 需要满足的2个条件才能继续
    while (writer_entered_ || reader_count_ == MAX_READER) {
        reader_.wait(latch);
    }
    reader_count_++;
}
void ReaderWriterLatch::RUnlock() {
    std::unique_lock<mutex_t> latch(mtx_);
    reader_count_--;
    if (writer_entered_) {
        if (reader_count_ == 0) {
            // 没有reader了，通知一个writer可以开始
            writer_.notify_all();
        }
    } else {
        if (reader_count_ == MAX_READER - 1) {
            // 唤醒因为超负荷（reader_count_ == MAX_READER）阻塞的reader
            reader_.notify_one();
        }
    }
}