#include <iostream>
#include <thread>
#include <vector>
#include <chrono>
#include <mutex>
#include <fstream>
#include <numeric>
#include <cmath>
#include <algorithm>

std::mutex cout_mutex;

void task(int id, int duration, std::vector<double>& wait_times, std::vector<double>& execution_times, std::chrono::time_point<std::chrono::high_resolution_clock> program_start) {
    auto start_wait = std::chrono::high_resolution_clock::now();
    std::this_thread::sleep_for(std::chrono::milliseconds(duration));
    auto end = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double, std::milli> wait_time = start_wait - program_start;
    std::chrono::duration<double, std::milli> elapsed = end - start_wait;

    std::lock_guard<std::mutex> lock(cout_mutex);
    wait_times[id] = wait_time.count();
    execution_times[id] = elapsed.count();
}

std::vector<double> calculate_statistics(std::vector<double>& data) {
    size_t num_elements_to_discard = data.size() / 10; // 10% dos melhores e piores resultados
    std::sort(data.begin(), data.end());
    data.erase(data.begin(), data.begin() + num_elements_to_discard); // Descartar os melhores
    data.erase(data.end() - num_elements_to_discard, data.end()); // Descartar os piores

    double mean = std::accumulate(data.begin(), data.end(), 0.0) / data.size();
    double sum = std::accumulate(data.begin(), data.end(), 0.0, [mean](double sum, double value) { return sum + std::pow(value - mean, 2); });
    double std_dev = std::sqrt(sum / data.size());

    return {mean, std_dev};
}

void run_test_case(int case_num, const std::vector<int>& durations, std::ofstream& data_file) {
    const int numTasks = durations.size();
    const int numIterations = 50;
    std::vector<double> all_wait_times;
    std::vector<double> all_execution_times;

    std::cout << "Starting test case " << case_num << std::endl;

    for (int iter = 0; iter < numIterations; ++iter) {
        std::vector<std::thread> tasks;
        std::vector<double> wait_times(numTasks, 0);
        std::vector<double> execution_times(numTasks, 0);

        auto program_start = std::chrono::high_resolution_clock::now();

        for (int i = 0; i < numTasks; ++i) {
            tasks.emplace_back(task, i, durations[i], std::ref(wait_times), std::ref(execution_times), program_start);
        }

        for (auto &t : tasks) {
            t.join();
        }

        // Adicionar resultados individuais ao vetor geral e imprimir
        for (int i = 0; i < numTasks; ++i) {
            all_wait_times.push_back(wait_times[i]);
            all_execution_times.push_back(execution_times[i]);
            std::cout << "Test Case " << case_num << ", Iteration " << iter << ", Task " << i
                      << " - Wait Time: " << wait_times[i] << " ms, Execution Time: " << execution_times[i] << " ms\n";
        }
    }

    std::vector<double> wait_time_stats = calculate_statistics(all_wait_times);
    std::vector<double> execution_time_stats = calculate_statistics(all_execution_times);

    double throughput = numTasks * numIterations / (std::accumulate(all_execution_times.begin(), all_execution_times.end(), 0.0) / 1000);

    std::cout << "Completed test case " << case_num << ": Mean Wait Time = " << wait_time_stats[0] << " ms, Std Dev Wait Time = " << wait_time_stats[1] << " ms, Mean Execution Time = " << execution_time_stats[0] << " ms, Std Dev Execution Time = " << wait_time_stats[1] << " ms, Throughput = " << throughput << " tasks/s\n";

    data_file << "Case " << case_num << "," << wait_time_stats[0] << "," << wait_time_stats[1] << ","
              << execution_time_stats[0] << "," << execution_time_stats[1] << "," << throughput << "\n";
}

int main() {
    std::ofstream data_file("test_data.csv");
    data_file << "TestCase,MeanWaitTime,StdDevWaitTime,MeanExecutionTime,StdDevExecutionTime,Throughput\n";

    std::vector<std::vector<int>> test_cases = {
        {100, 500, 200, 600, 300, 700, 400, 800},
        {300, 100, 400, 200, 500, 300, 600, 400},
        {50, 250, 150, 350, 250, 450, 350, 550},
        {600, 200, 700, 300, 800, 400, 900, 500},
        {1000, 200, 900, 300, 800, 400, 700, 500},
        {120, 480, 360, 240, 600, 420, 180, 300},
        {500, 1000, 1500, 200, 2500, 300, 3500, 400}
    };

    for (size_t i = 0; i < test_cases.size(); ++i) {
        run_test_case(i + 1, test_cases[i], data_file);
    }

    data_file.close();
    return 0;
}