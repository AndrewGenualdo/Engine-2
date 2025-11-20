#pragma once
#include <chrono>
#include <map>
#include <string>
#include <stdexcept>

class Profiler {
public:
    struct Entry {
        using clock = std::chrono::high_resolution_clock;

        clock::time_point start_time;
        bool started = false;

        float accumulated_ms = 0.0;//total duration in milliseconds
        int frame_count = 0;
        float last_average_ms = 0.0;//most recent average (ms)
        clock::time_point last_update = clock::now();

        void start() {
            started = true;
            start_time = clock::now();
        }

        void end() {
            if (!started)
                throw std::runtime_error("end() called before start()");

            const auto end_time = clock::now();

            const std::chrono::duration<double, std::milli> diff = end_time - start_time;
            accumulated_ms += static_cast<float>(diff.count());
            frame_count++;
            started = false;

            if (frame_count == 1 && accumulated_ms == diff.count()) {
                last_average_ms = static_cast<float>(diff.count());
                last_update = end_time;
                return;
            }

            update_average_if_needed(end_time);
        }

        void update_average_if_needed(const clock::time_point& now) {
            if (const std::chrono::duration<float> elapsed = now - last_update; elapsed.count() >= 1.0) {
                if (frame_count > 0)
                    last_average_ms = accumulated_ms / static_cast<float>(frame_count);
                else
                    last_average_ms = 0.0;

                accumulated_ms = 0.0;
                frame_count = 0;
                last_update = now;
            }
        }

        [[nodiscard]] float duration_ms() const {
            return last_average_ms;
        }

        [[nodiscard]] float duration_micro() const {
            return last_average_ms * 1000.0f;
        }

        [[nodiscard]] float duration_nano() const {
            return last_average_ms * 1000000.0f;
        }
    };

    static void create(const std::string& name) {
        auto& map = getMap();
        map[name] = Entry{};
    }

    static Entry& get(const std::string& name) {
        auto& map = getMap();
        const auto it = map.find(name);
        if (it == map.end())
            throw std::runtime_error("Profiler entry '" + name + "' not found.");
        return it->second;
    }

    static std::map<std::string, Entry>& getMap() {
        static std::map<std::string, Entry> entries;
        return entries;
    }
};
