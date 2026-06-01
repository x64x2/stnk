#include <stnik/at/job.hpp>
#include <stnik/at/service.hpp>

stn::at_job::~at_job()
{
    unregister();
}

void stn::at_job::unregister()
{
    if (death_sub)
    {
        death_sub->unregister_job(this);
        death_sub = 0;
    }
}

stn::at_job::at_job()
{      
    death_sub(0)
}


void stn::at_job::death_sub(at_service *sp)
{
    death_sub = sp;
}
//vim: set ts=4 et
