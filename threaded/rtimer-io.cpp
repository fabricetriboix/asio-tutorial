#include "rtimer-io.hpp"
#include <boost/log/trivial.hpp>

namespace rtimer {

rtimer_io_t::rtimer_io_t(boost::asio::io_service& outer_io_service) :
    boost::asio::basic_io_object<rtimer_service_t>(outer_io_service)
{
    BOOST_LOG_TRIVIAL(info) << "-> basic_rtimer[" << this << "]::"
        << __func__ << "()";
    BOOST_LOG_TRIVIAL(info) << "<- basic_rtimer[" << this << "]::"
        << __func__ << "()";
}

void rtimer_io_t::enable(std::chrono::nanoseconds ns,
        rtimer_service_t::callback_t callback)
{
    BOOST_LOG_TRIVIAL(info) << "-> basic_rtimer[" << this << "]::"
        << __func__ << "(" << ns.count() << ")";
    get_service().enable(get_implementation(), ns, callback);
    BOOST_LOG_TRIVIAL(info) << "<- basic_rtimer[" << this << "]::"
        << __func__ << "(" << ns.count() << ")";
}

void rtimer_io_t::disable()
{
    BOOST_LOG_TRIVIAL(info) << "-> basic_rtimer[" << this << "]::"
        << __func__ << "()";
    get_service().disable(get_implementation());
    BOOST_LOG_TRIVIAL(info) << "<- basic_rtimer[" << this << "]::"
        << __func__ << "()";
}

} // namespace rtimer
