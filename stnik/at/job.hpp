#ifndef stnik_AT_JOB_H
#define stnik_AT_JOB_H

#include <stnik/time_value.hpp>

namespace stn {

class at_service; 

class at_job
{
public:
    virtual ~at_job();

protected:
    at_job();

public:
    /**
      * The death_subscribe method is used by the
      * stn::at_service::register_job method to as to be notified
      * of this objects destruction.  The destructor will call
      * stn::at_service::unregister_job in that case.
      */
    void death_subscribe(at_service *sp);
    virtual void process() = 0;
    void unregister();
};
}