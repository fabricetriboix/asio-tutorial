#include "rtimer-service.hpp"
#include <boost/log/trivial.hpp>

namespace rtimer {

boost::asio::io_service::id rtimer_service_t::id;

rtimer_service_t::rtimer_service_t(boost::asio::io_service& outer_io_service) :
    boost::asio::io_service::service(outer_io_service)
{
    BOOST_LOG_TRIVIAL(info) << "-> rtimer_service_t[" << this << "]::"
        << __func__ << "()";
    BOOST_LOG_TRIVIAL(info) << "<- rtimer_service_t[" << this << "]::"
        << __func__ << "()";
}

rtimer_service_t::~rtimer_service_t()
{
    BOOST_LOG_TRIVIAL(info) << "-> rtimer_service_t[" << this << "]::"
        << __func__ << "()";
    BOOST_LOG_TRIVIAL(info) << "<- rtimer_service_t[" << this << "]::"
        << __func__ << "()";
}

void rtimer_service_t::construct(implementation_type& impl)
{
    BOOST_LOG_TRIVIAL(info) << "-> rtimer_service_t[" << this << "]::"
        << __func__ << "(" << &impl << ")";
    impl.make();
    BOOST_LOG_TRIVIAL(info) << "<- rtimer_service_t[" << this << "]::"
        << __func__ << "(" << &impl << ")";
}

void rtimer_service_t::destroy(implementation_type& impl)
{
    BOOST_LOG_TRIVIAL(info) << "-> rtimer_service_t[" << this << "]::"
        << __func__ << "(" << &impl << ")";
    impl.unmake();
    BOOST_LOG_TRIVIAL(info) << "<- rtimer_service_t[" << this << "]::"
        << __func__ << "(" << &impl << ")";
}

void rtimer_service_t::enable(implementation_type& impl,
        std::chrono::nanoseconds ns, callback_t callback)
{
    BOOST_LOG_TRIVIAL(info) << "-> rtimer_service_t[" << this << "]::"
        << __func__ << "(" << &impl << ", " << ns.count() << ")";
    impl.enable(ns, std::bind(&rtimer_service_t::handler, this, callback));
    BOOST_LOG_TRIVIAL(info) << "<- rtimer_service_t[" << this << "]::"
        << __func__ << "(" << &impl << ", " << ns.count() << ")";
}

void rtimer_service_t::disable(implementation_type& impl)
{
    BOOST_LOG_TRIVIAL(info) << "-> rtimer_service_t[" << this << "]::"
        << __func__ << "(" << &impl << ")";
    impl.disable();
    BOOST_LOG_TRIVIAL(info) << "<- rtimer_service_t[" << this << "]::"
        << __func__ << "(" << &impl << ")";
}

void rtimer_service_t::shutdown_service()
{
    BOOST_LOG_TRIVIAL(info) << "-> rtimer_service_t[" << this << "]::"
        << __func__ << "()";
    BOOST_LOG_TRIVIAL(info) << "<- rtimer_service_t[" << this << "]::"
        << __func__ << "()";
}

void rtimer_service_t::handler(callback_t callback)
{
    BOOST_LOG_TRIVIAL(info) << "-> rtimer_service_t[" << this << "]::"
        << __func__ << "()";

    /** @note **Very important!** Do not call the client's callback directly.
     *        This might breaks it as it might not be MT-safe, and it might
     *        break its expectations. The callback must go through the
     *        client's `io_service`, in order to let it control how the
     *        callback is executed.
     */
    get_io_service().post(callback);

    BOOST_LOG_TRIVIAL(info) << "<- rtimer_service_t[" << this << "]::"
        << __func__ << "()";
}

} // namespace rtimer
