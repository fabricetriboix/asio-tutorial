#pragma once

#include <ctime>
#include <csignal>
#include <functional>
#include <chrono>
#include <boost/noncopyable.hpp>

namespace rtimer {

void rtimer_handler(union sigval arg);

/** Class representing the implementation of an I/O object
 *
 * This class actually implements the behaviour required (communicate over the
 * network, communicate with a device, etc. You will have one instance of this
 * class for each I/O object you need.
 *
 * This class does not derive from any Boost.Asio classes, but must comply with
 * certain requirements.
 */
class rtimer_impl_t final : boost::noncopyable
{
public:
    /** Boost.Asio requires a public default constructor */
    rtimer_impl_t();

    /** Boost.Asio requires a public destructor */
    ~rtimer_impl_t();

    /** Construct the object
     *
     * This method can be called by your I/O service when a new I/O object
     * is created.
     */
    void make();

    /** Destruct the object
     *
     * This method can be called by your I/O service when an I/O object is
     * destroyed.
     */
    void unmake();

    using callback_t = std::function<void()>;

    /** Enable the timer
     *
     * This method will be mirrored by your I/O object. You can have as many
     * such methods as you need to implement your custom service.
     */
    void enable(std::chrono::nanoseconds ns, callback_t callback);

    /** Disable the timer
     *
     * Again, this method will be mirrored by your I/O object.
     */
    void disable();

private:
    bool initialised_;
    timer_t timer_;
    callback_t callback_;

    friend void rtimer_handler(union sigval);
};


} // namespace rtimer
