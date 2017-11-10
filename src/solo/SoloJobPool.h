/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#pragma once

#include "SoloCommon.h"
#include "SoloSpinLock.h"
#include <functional>
#include <future>

namespace solo
{
    class Job
    {
    public:
        SL_DISABLE_COPY_AND_MOVE(Job)

        virtual ~Job(){}

        bool isDone() const { return done; }

        virtual void update() = 0;

    protected:
        Job() {}

        bool done = false;
    };

    template <class T>
    class JobBase final: public Job
    {
    public:
        using Producer = std::function<sptr<T>()>;
        using Producers = vec<Producer>;
        using Consumer = std::function<void(const vec<sptr<T>> &)>;

        JobBase(const vec<Producer> &funcs, const Consumer &onDone):
            callback(onDone)
        {
            for (auto &func : funcs)
            {
                auto future = std::async(std::launch::async, func);
                futures.push_back(std::move(future));
                results.push_back(nullptr);
            }
        }

        void update() override final
        {
            auto readyCount = 0;
            auto i = 0;
            for (auto &future : futures)
            {
                if (future.valid())
                {
                    const auto status = future.wait_for(std::chrono::nanoseconds(0));
                    if (status == std::future_status::ready)
                        results[i] = future.get();
                }
                else
                {
                    if (results[i])
                        readyCount++;
                    else
                        throw EngineException("Failed to obtain job result"); // TODO less generic message
                }
                i++;
            }

            if (readyCount == futures.size())
            {
                callback(results);
                done = true;
            }
        }

    private:
        vec<std::future<sptr<T>>> futures;
        vec<sptr<T>> results;
        Consumer callback;
    };

    class JobPool final
    {
    public:
        SL_DISABLE_COPY_AND_MOVE(JobPool)

        JobPool() {}

        bool hasActiveJobs() const { return anyActiveJobs; }
        void addJob(sptr<Job> job);
        void update();

    private:
        list<sptr<Job>> jobs;
        bool anyActiveJobs = false;
        SpinLock lock;
    };
}
