#ifndef THREADED_PROCESS_HPP
#define THREADED_PROCESS_HPP

#include <thread>
#include <mutex>
#include <memory>
#include <chrono>
#include <vector>
#include <functional>

class ThreadedProcess
{
  private:
    bool stop_thread_;
    std::unique_ptr<std::thread> thread_;
    std::chrono::milliseconds interval_;

  public:
    ThreadedProcess() : stop_thread_(false), thread_(nullptr)
    {
    }

    ~ThreadedProcess()
    {
        stop_thread_ = true;
        if (thread_ && thread_->joinable())
        {
            thread_->join();
            thread_.reset(nullptr);
        }
    }

    void start(std::size_t interval_ms = 0)
    {
        if (!thread_)
        {
            stop_thread_ = false;
            if (interval_ms == 0)
            {
                thread_ = std::make_unique<std::thread>(
                    [&]() {
                        while (!stop_thread_)
                        {
                            update();
                        }
                    });
            }
            else
            {
                interval_ = std::chrono::milliseconds(interval_ms);
                thread_ = std::make_unique<std::thread>(
                    [&]() {
                        while (true)
                        {
                            if (stop_thread_)
                                return;
                            std::this_thread::sleep_for(interval_);
                            if (stop_thread_)
                                return;
                            update();
                        }
                    });
            }
        }
    }

    void stop()
    {
        stop_thread_ = true;
        if (thread_ && thread_->joinable())
        {
            thread_->join();
            thread_.reset(nullptr);
        }
    }

    virtual void update() = 0;
};

#endif // !THREADED_PROCESS_HPP
