/*
 * Copyright (c) Aleksey Fedotov
 * MIT license
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
        Job(const Job &other) = delete;
        Job(Job &&other) = delete;
        virtual ~Job() = default;

        auto operator=(const Job &other) -> Job & = delete;
        auto operator=(Job &&other) -> Job & = delete;

        bool isDone() const
        {
            return done_;
        }

        virtual void update() = 0;

    protected:
        Job() = default;

        bool done_ = false;
    };

    template <class T>
    class JobBase final: public Job
    {
    public:
        using Producer = std::function<sptr<T>()>;
        using Producers = vec<Producer>;
        using Consumer = std::function<void(const vec<sptr<T>> &)>;

        JobBase(const vec<Producer> &funcs, const Consumer &onDone):
            callback_(onDone)
        {
            for (auto &func : funcs)
            {
                auto future = std::async(std::launch::async, func);
                futures_.push_back(std::move(future));
                results_.push_back(nullptr);
            }
        }

        void update() override final
        {
            auto readyCount = 0;
            auto i = 0;
            for (auto &future : futures_)
            {
                if (future.valid())
                {
                    const auto status = future.wait_for(std::chrono::nanoseconds(0));
                    if (status == std::future_status::ready)
                        results_[i] = future.get();
                }
                else
                {
                    panicIf(!results_[i], "Unable to obtain job result");
                    readyCount++;
                }
                i++;
            }

            if (readyCount == futures_.size())
            {
                callback_(results_);
                done_ = true;
            }
        }

    private:
        vec<std::future<sptr<T>>> futures_;
        vec<sptr<T>> results_;
        Consumer callback_;
    };

    class JobPool
    {
    public:
        JobPool() = default;
        JobPool(const JobPool &other) = delete;
        JobPool(JobPool &&other) = delete;
        virtual ~JobPool() = default;

        auto operator=(const JobPool &other) -> JobPool & = delete;
        auto operator=(JobPool &&other) -> JobPool & = delete;

        bool hasActiveJobs() const
        {
            return anyActiveJobs_;
        }
        void addJob(sptr<Job> job);
        void update();

    private:
        list<sptr<Job>> jobs_;
        bool anyActiveJobs_ = false;
        SpinLock lock_;
    };
}
