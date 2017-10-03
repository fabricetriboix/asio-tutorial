#include "rtimer-impl.hpp"
#include <cstring>
#include <cerrno>
#include <stdexcept>
#include <boost/assert.hpp>
#include <boost/log/trivial.hpp>
#include <boost/system/error_code.hpp>
#include <boost/system/system_error.hpp>

namespace rtimer {

void rtimer_handler(union sigval arg)
{
    rtimer_impl_t* impl = static_cast<rtimer_impl_t*>(arg.sival_ptr);
    BOOST_LOG_TRIVIAL(info) << "-> rtimer_impl_t[" << impl << "]::"
        << __func__ << "(" << arg.sival_ptr << ")";
    impl->callback_();
    BOOST_LOG_TRIVIAL(info) << "<- rtimer_impl_t[" << impl << "]::"
        << __func__ << "(" << arg.sival_ptr << ")";
}

rtimer_impl_t::rtimer_impl_t() : initialised_(false), callback_(nullptr)
{
    BOOST_LOG_TRIVIAL(info) << "-> rtimer_impl_t[" << this << "]::"
        << __func__ << "()";
    BOOST_LOG_TRIVIAL(info) << "<- rtimer_impl_t[" << this << "]::"
        << __func__ << "()";
}

rtimer_impl_t::~rtimer_impl_t()
{
    BOOST_LOG_TRIVIAL(info) << "-> rtimer_impl_t[" << this << "]::"
        << __func__ << "()";
    BOOST_ASSERT(!initialised_);
    BOOST_LOG_TRIVIAL(info) << "<- rtimer_impl_t[" << this << "]::"
        << __func__ << "()";
}

void rtimer_impl_t::make()
{
    BOOST_LOG_TRIVIAL(info) << "-> rtimer_impl_t[" << this << "]::"
        << __func__ << "()";
    BOOST_ASSERT(!initialised_);

    // Create the POSIX timer
    ::sigevent sigev;
    std::memset(&sigev, 0, sizeof(sigev));
    sigev.sigev_notify = SIGEV_THREAD;
    sigev.sigev_notify_function = rtimer_handler;
    sigev.sigev_value.sival_ptr = static_cast<void*>(this);
    int ret = ::timer_create(CLOCK_MONOTONIC, &sigev, &timer_);
    if (ret < 0) {
        boost::system::system_error e(boost::system::error_code(errno,
                    boost::system::get_system_category()),
                "timer_create() failed");
        throw e;
    }
    initialised_ = true;

    BOOST_LOG_TRIVIAL(info) << "<- rtimer_impl_t[" << this << "]::"
        << __func__ << "()";
}

void rtimer_impl_t::unmake()
{
    BOOST_LOG_TRIVIAL(info) << "-> rtimer_impl_t[" << this << "]::"
        << __func__ << "()";
    BOOST_ASSERT(initialised_);

    // Delete the POSIX timer
    (void)::timer_delete(timer_);
    initialised_ = false;

    BOOST_LOG_TRIVIAL(info) << "<- rtimer_impl_t[" << this << "]::"
        << __func__ << "()";
}

void rtimer_impl_t::enable(std::chrono::nanoseconds ns, callback_t callback)
{
    BOOST_LOG_TRIVIAL(info) << "-> rtimer_impl_t[" << this << "]::" << __func__
        << "(" << ns.count() << ")";
    BOOST_ASSERT(initialised_);
    if (callback == nullptr) {
        throw std::invalid_argument("Callback must not be null");
    }
    callback_ = callback;

    // Enable the POSIX timer
    ::itimerspec its;
    std::memset(&its, 0, sizeof(its));
    its.it_value.tv_sec  = ns.count() / 1000000000;
    its.it_value.tv_nsec = ns.count() % 1000000000;
    its.it_interval.tv_sec = its.it_value.tv_sec;
    its.it_interval.tv_nsec = its.it_value.tv_nsec;
    int ret = ::timer_settime(timer_, 0, &its, nullptr);
    if (ret < 0) {
        callback_ = nullptr;
        boost::system::system_error e(boost::system::error_code(errno,
                    boost::system::get_system_category()),
                "timer_settime() failed");
        throw e;
    }

    BOOST_LOG_TRIVIAL(info) << "<- rtimer_impl_t[" << this << "]::" << __func__
        << "(" << ns.count() << ")";
}

void rtimer_impl_t::disable()
{
    BOOST_LOG_TRIVIAL(info) << "-> rtimer_impl_t[" << this << "]::" << __func__
        << "()";

    // Disable the POSIX timer
    ::itimerspec its;
    std::memset(&its, 0, sizeof(its));
    int ret = ::timer_settime(timer_, 0, &its, nullptr);
    if (ret < 0) {
        callback_ = nullptr;
        boost::system::system_error e(boost::system::error_code(errno,
                    boost::system::get_system_category()),
                "timer_settime() failed");
        throw e;
    }
    callback_ = nullptr;

    BOOST_LOG_TRIVIAL(info) << "<- rtimer_impl_t[" << this << "]::" << __func__
        << "()";
}

} // namespace rtimer
