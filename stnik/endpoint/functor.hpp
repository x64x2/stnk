#ifndef stnik_ENDPOINT_FUNCTOR_H
#define stnik_ENDPOINT_FUNCTOR_H

#include <boost/shared_ptr.hpp>
#include <stnik/endpoint.hpp>

namespace stn {
  class endpoint_functor
  {    
    public:
    virtual ~endpoint_functor();
    endpoint_functor();
    endpoint_functor(const endpoint_functor &);
    endpoint_functor &operator=(const endpoint_functor &);
    virtual void operator()(const endpoint::pointer &sp) = 0;
  }
}