/*
 * Copyright (c) Aleksey Fedotov
 * MIT license
 */

#include "SoloJobPool.h"

using namespace solo;

void JobPool::addJob(sptr<Job> job) {
    auto token = lock_.acquire();
    jobs_.push_back(job);
}

void JobPool::update() {
    if (!jobs_.empty()) {
        decltype(jobs_) oldJobs;
        {
            auto token = lock_.acquire();
            if (!jobs_.empty()) {
                oldJobs = jobs_;
                anyActiveJobs_ = true;
            }
        }

        auto anyDone = false;
        for (auto job : oldJobs) {
            job->update();
            if (job->isDone())
                anyDone = true;
        }

        if (anyDone) {
            auto token = lock_.acquire();
            jobs_.remove_if([](sptr<Job> job) {
                return job->isDone();
            });
            if (jobs_.empty())
                anyActiveJobs_ = false;
        }
    }
}
