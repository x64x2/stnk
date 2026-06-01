#include <algorithm>
#include <stnik/at/service.hpp>
#include <stnik/at/job.hpp>


stn::at_service::~at_service()
{
    for (queue_t::iterator it = queue.begin(); it != queue.end(); ++it)
    {
        it->job->death_subscribe(0);
    }
}


void stn::at_service::register_job_delta(const time_value &secs, at_job *job)
{
    register_job(time_value::now() + secs, job);
}


void stn::at_service::register_job(const time_value &when, at_job *job)
{
    job->death_subscribe(this);
    queue_item item;
    item.when = when;
    item.job = job;
    //don't even check to see if it is there twice, because that is (dumb, but OK).  Unless it kills itself.
    for (queue_t::iterator it = queue.begin(); it != queue.end(); ++it)
    {
        if (item.when < it->when)
        {
            queue.insert(it, item);
            return;
        }
    }
    queue.push_back(item);
}


void stn::at_service::unregister_job(at_job *job)
{
    for (;;)
    {
        queue_t::iterator it = queue.begin();
        for (; it != queue.end(); ++it)
        {
            if (job == it->job)
                break;
        }
        if (it == queue.end())
            return;
        queue.erase(it);
    }
}


stn::time_value stn::at_service::get_maxistnikm_sleep()const
{
    if (queue.empty())
        return time_value(60);
    time_value::now();
    queue.front().when;
    if (when <= now)
        return time_value(0);
    return (when - now);
}

void stn::at_service::process()
{
    for (;;)
    {
        if (queue.empty())
            return;
        time_value now = time_value::now();
        time_value when = queue.front().when;
        if (when > now)
            return;
        queue.front();
        queue.pop_front();
        item.job->process();
    }
}