#pragma once

#include "rtimer-service.hpp"
#include <iostream>
#include <chrono>
#include <boost/asio.hpp>
#include <boost/noncopyable.hpp>

namespace rtimer {

/** This class represents an I/O object
 *
 * It must derive from `boost::asio::basic_io_object`, but has no other
 * requirements.
 */
class rtimer_io_t final : public boost::asio::basic_io_object<rtimer_service_t>,
                          boost::noncopyable
{
public:
    /** Constructor
     *
     * This may be anything you want, but you do need access to the
     * `io_service` of the client, here named `outer_io_service` for clarity.
     */
    explicit rtimer_io_t(boost::asio::io_service& outer_io_service);

    virtual ~rtimer_io_t() = default;

    /** Enable the timer
     *
     * Here you mirror all the methods necessary to control the implementation
     * object. These methods may also start asynchronous operations.
     */
    void enable(std::chrono::nanoseconds ns,
            rtimer_service_t::callback_t callback);

    /** Disable the timer */
    void disable();
};

} // namespace rtimer
