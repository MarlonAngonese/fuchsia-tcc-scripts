#include <iostream>
#include <thread>
#include <vector>
#include <chrono>
#include <mutex>

std::mutex cout_mutex;
auto program_start = std::chrono::high_resolution_clock::now();

void task(int id, int duration) {
    auto start_wait = std::chrono::high_resolution_clock::now();

    // Simula trabalho pesado
    std::this_thread::sleep_for(std::chrono::milliseconds(duration));

    auto end = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double, std::milli> wait_time = start_wait - program_start;
    std::chrono::duration<double, std::milli> elapsed = end - start_wait;

    std::lock_guard<std::mutex> lock(cout_mutex);
    std::cout << "Task " << id << " - Wait Time: " << wait_time.count() << " ms, "
              << "Execution Time (Latency): " << elapsed.count() << " ms\n";
}

int main() {
    const int numTasks = 5;

    // Criando e iniciando tarefas
    std::vector<std::thread> tasks;
    for (int i = 0; i < numTasks; ++i) {
        tasks.emplace_back(task, i, 100 * (i + 1));
    }

    // Aguardando todas as tarefas serem concluídas
    for (auto &t : tasks) {
        t.join();
    }

    auto program_end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> total_program_time = program_end - program_start;
    std::cout << "Total Program Execution Time: " << total_program_time.count() << " ms\n";
    std::cout << "Throughput: " << numTasks / (total_program_time.count() / 1000) << " tasks/s\n";

    return 0;
}