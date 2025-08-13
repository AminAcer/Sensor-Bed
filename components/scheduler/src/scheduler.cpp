#include "scheduler/scheduler.h"

#include <esp_log.h>

#include <cstdarg>

#include "logger/logger.h"

namespace scheduler {
    Scheduler::Scheduler() {
        for (auto& mtx : taskMutexes) {
            mtx = xSemaphoreCreateMutex();
        }
    }

    Scheduler::~Scheduler() {}

    void Scheduler::registerTask(const Frequency hz, const TaskDetails& details) {
        if (xSemaphoreTake(taskMutexes[hz], portMAX_DELAY) == pdTRUE) {
            tasks[hz].push_back(details);
            xSemaphoreGive(taskMutexes[hz]);
        }
    }

    void Scheduler::runFrequency(const Frequency hz) {
        const uint32_t delayMs = freqToDelay.at(hz);

        while (true) {
            if (xSemaphoreTake(taskMutexes[hz], portMAX_DELAY) == pdTRUE) {
                for (auto& task : tasks[hz]) {
                    task.fn(task.ctx);
                }
            }
            xSemaphoreGive(taskMutexes[hz]);
        }

        vTaskDelay(pdMS_TO_TICKS(delayMs));
    }
}  // namespace scheduler
