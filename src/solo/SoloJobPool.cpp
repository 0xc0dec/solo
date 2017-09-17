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

bool JobPool::update()
{
    if (!jobs.empty())
    {
        auto token = lock.acquire();
        if (!jobs.empty())
        {
            for (auto job : jobs)
                job->updateStatus();

            const auto wasEmpty = jobs.empty();
            jobs.remove_if([](sptr<Job> job) { return job->isDone(); });

            if (jobs.empty() && !wasEmpty)
                return true;
        }
    }

    return false;
}
