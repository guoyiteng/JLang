#include <iostream>
#include <unordered_map>
#include <thread>
#include <mutex>

std::unordered_map<std::thread::id, std::string> m;
std::mutex mtx;

void task() {
        std::unique_lock<std::mutex> lockGuard(mtx);
        // mtx.lock();

        std::cout << m.size() << ", " << std::this_thread::get_id() << std::endl;
        m.emplace(std::this_thread::get_id(), "asdf");
        // mtx.unlock();
}

int main(int argc, char** argv) {
    std::cout << "hello" << std::endl;
    {
        std::unique_lock<std::mutex>(mtx);

        m.emplace(std::this_thread::get_id(), "asdfasdf");

    }
    std::thread threads[100];
    for (int i = 0; i < 100; ++i) {
        threads[i] = std::thread(task);
    }
    std::this_thread::sleep_for(std::chrono::seconds(1));
    for (int i = 0; i < 100; ++i) {
        threads[i].join();
    }

}