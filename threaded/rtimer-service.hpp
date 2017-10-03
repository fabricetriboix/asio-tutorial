#pragma once

#include "rtimer-impl.hpp"
#include <functional>
#include <boost/asio.hpp>
#include <boost/noncopyable.hpp>

namespace rtimer {

/** This class implements your I/O service
 *
 * This class must derive from `boost::asio::io_service::service` and must
 * comply with a few requirements.
 *
 * You will have only one instance of this class being created per
 * `io_service`. This one object will communicate on one side with the
 * I/O objects, and on the other with your service implementation.
 *
 * So although there is only one I/O service object, there are as many
 * I/O objects as "implementation objects" with a one-to-one relationship
 * between them.
 */
class rtimer_service_t final : public boost::asio::io_service::service,
                               boost::noncopyable
{
public:
    /** Unique I/O service id, required (and managed) by Boost.Asio */
    static boost::asio::io_service::id id;

    /** Boost.Asio requires you to define the `implementation_type`
     *
     * This type is the type of your "implementation objects".
     */
    using implementation_type = rtimer_impl_t;

    /** Boost.Asio requires a constructor with a single argument
     *
     * The argument must be an `io_service&`.
     */
    explicit rtimer_service_t(boost::asio::io_service& outer_io_service);

    /** Boost.Asio requires a public destructor */
    virtual ~rtimer_service_t();

    /** Construct an implementation object
     *
     * When an I/O object of yours is instanciated, Boost.Asio will create
     * an implementation object which will be the mirror of the I/O object.
     * It will then call the `construct()` method of your I/O service with
     * `impl` being a reference to the implementation object that Boost.Asio
     * just created.
     *
     * This method is required by Boost.Asio. Since Boost.Asio requires the
     * `implementation_type` to have a default constructor, this is the
     * opportunity for you to pass some arguments, if need be.
     *
     * Conversly, if your implementation object does not need any argument and
     * is already fully constructed by its constructor, this method can be
     * left empty.
     */
    void construct(implementation_type& impl);

    /** Destroy an implementation object
     *
     * This method is required by Boost.Asio and it the opposite of
     * `construct()`. You guessed that Boost.Asio will call this method when
     * the I/O object corresponding to `impl` is being destroyed.
     */
    void destroy(implementation_type& impl);

    using callback_t = std::function<void()>;

    /** Link between the I/O object and the implementation object */
    void enable(implementation_type& impl, std::chrono::nanoseconds ns,
            callback_t callback);

    /** Link between the I/O object and the implementation object */
    void disable(implementation_type& impl);

private:
    /** Shutdown this I/O service object
     *
     * This method is required by Boost.Asio. It is called when the
     * `io_service` owning your I/O service object is destroyed.
     *
     * Please note that at this stage, all the implementation objects
     * should have been destroyed already. So this method is purely for the
     * internals of your I/O service object.
     */
    virtual void shutdown_service() override;

    /** Timer callback */
    void handler(callback_t callback);
};

} // namespace rtimer
