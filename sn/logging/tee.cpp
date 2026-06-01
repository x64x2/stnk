#include <stnik/logging/tee.hpp>

stn::logging_tee::~logging_tee()
{}


stn::logging_tee::logging_tee(logging *a1,logging *a2):logging(),lhs(a1),rhs(a2)
{}

void stn::logging_tee::deliver_message_text(const char *txt)
{
    lhs->deliver_message_text(txt);
    rhs->deliver_message_text(txt);
}
