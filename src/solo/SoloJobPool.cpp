/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#include "SoloJobPool.h"

using namespace solo;

void JobPool::addJob(sptr<Job> job)
{
    auto token = lock.acquire();
    jobs.push_back(job);
}

void JobPool::update()
{
    if (!jobs.empty())
    {
        decltype(jobs) oldJobs;
        {
            auto token = lock.acquire();
            if (!jobs.empty())
                oldJobs = jobs;
        }

        auto anyDone = false;
        for (auto job : oldJobs)
        {
            job->updateStatus();
            if (job->isDone())
                anyDone = true;
        }

        if (anyDone)
        {
            auto token = lock.acquire();
            jobs.remove_if([](sptr<Job> job) { return job->isDone(); });
        }
    }
}
