#pragma once

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#include <array>
#include <mutex>
#include <string>
#include <unordered_map>
#include <vector>

namespace scheduler {
    enum Frequency { FREQ_1HZ = 0, FREQ_5HZ, FREQ_10HZ, FREQ_20HZ, FREQ_TOTAL };

    const std::unordered_map<Frequency, uint32_t> freqToDelay{
        {FREQ_1HZ, 1000},
        {FREQ_5HZ, 200},
        {FREQ_10HZ, 100},
        {FREQ_20HZ, 50},
    };

    struct TaskDetails {
        /// Name for the task
        std::string name;
        /// Function to run
        std::function<void(void*)> fn;
        /// Context pointer for the function
        void* ctx;
    };

    class Scheduler {
       public:
        /// @brief Constructor
        ///
        Scheduler();
        ~Scheduler();

        // === Rule Of 5 ===
        Scheduler(const Scheduler&) = delete;             // Copy constructor
        Scheduler& operator=(const Scheduler&) = delete;  // Copy assignment
        Scheduler(Scheduler&&) = delete;                  // Move constructor
        Scheduler& operator=(Scheduler&&) = delete;       // Move assignment

        /// @brief Register a task to a specific frequency
        ///
        /// @param hz: Rate at which to run the task
        /// @param details: Struct containing everything needed to run a function on a thread
        void registerTask(const Frequency hz, const TaskDetails& details);

       private:
        /// @brief Will execute all tasks registered to this frequency
        ///
        /// @param hz: Specific frequency being executed
        void runFrequency(const Frequency hz);

        /// @brief Container that stores all tasks
        std::array<std::vector<TaskDetails>, FREQ_TOTAL> tasks;
        std::array<SemaphoreHandle_t, FREQ_TOTAL> taskMutexes;

        /// @brief Mutex to protect data
        std::mutex m;
    };
}  // namespace scheduler
