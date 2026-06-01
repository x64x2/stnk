#ifndef stnik_AT_SERVICE_H
#define stnik_AT_SERVICE_H

#include <deque>
#include <stnik/time_value.hpp>

namespace stn {

class at_job; 

class at_service
{
public:
    virtual ~at_service();
    at_service();

    void register_job(const time_value &when, at_job *jp);

    void register_job_delta(const time_value &seconds, at_job *jp);
    void unregister_job(at_job *jp);
    time_value get_maxistnikm_sleep() const;

    void process();
    bool empty() const { return queue.empty(); 
    }

private:
    typedef std::deque<queue_item> queue_t;
    queue_t queue;

    at_service(const at_service &);
    at_service &operator=(const at_service &);
}}
