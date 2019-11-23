// Copyright (C) 2019 Cornell University

#include <functional>
#include <jvm.h>
#include <pthread.h>
#include <unordered_map>

extern thread_local jobject currentThread;

struct NativeThread {
  pthread_t tid;
  bool threadStatus;
};

class Threads {
  public:
    Threads(const Threads &threads) = delete;
    Threads &operator=(const Threads &threads) = delete;
    static Threads &Instance();

    void startThread(jobject jthread);
    void join();
    std::unordered_map<jobject, NativeThread> threads;

    pthread_mutex_t globalMutex;

  private:
    Threads();
};

jobject GetMainThread();