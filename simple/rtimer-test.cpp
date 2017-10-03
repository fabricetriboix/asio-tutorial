#include "rtimer-io.hpp"
#include <csignal>
#include <cstring>
#include <iostream>
#include <chrono>
#include <boost/asio.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/trivial.hpp>

boost::scoped_ptr<boost::asio::io_service::work> g_work;

void sigterm_handler(int signum)
{
    BOOST_LOG_TRIVIAL(warning) << "Caught signal "
        << signum <<", terminating...";
    g_work.reset();
}

int main()
{
    struct sigaction sa;
    std::memset(&sa, 0, sizeof(sa));
    sa.sa_handler = sigterm_handler;
    ::sigaction(SIGINT, &sa, nullptr);
    ::sigaction(SIGTERM, &sa, nullptr);

#ifdef WITH_TRACES
    boost::log::core::get()->set_filter(
            boost::log::trivial::severity >= boost::log::trivial::info
            );
#endif

    {
        boost::asio::io_service io_service;
        g_work.reset(new boost::asio::io_service::work(io_service));

        rtimer::rtimer_io_t my_timer(io_service);
        my_timer.enable(std::chrono::milliseconds(500), []() {
                BOOST_LOG_TRIVIAL(warning) << "TICK";
            });

        rtimer::rtimer_io_t my_timer2(io_service);
        my_timer2.enable(std::chrono::milliseconds(333), []() {
                BOOST_LOG_TRIVIAL(warning) << "TOCK";
            });

        BOOST_LOG_TRIVIAL(warning) << "***  Starting io_service.run()  ***";
        io_service.run();
        BOOST_LOG_TRIVIAL(warning) << "***  io_service.run() has returned  ***";
    }

    BOOST_LOG_TRIVIAL(warning) << "Terminated";
    return 0;
}
