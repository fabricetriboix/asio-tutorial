#include "rtimer-io.hpp"
#include <csignal>
#include <cstring>
#include <iostream>
#include <chrono>
#include <thread>
#include <boost/asio.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>

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

    boost::log::add_common_attributes();
#ifdef WITH_TRACES
    boost::log::core::get()->set_filter(
            boost::log::trivial::severity >= boost::log::trivial::info
            );
#endif

    BOOST_LOG_TRIVIAL(warning) << "main: starting";

    {
        boost::asio::io_service io_service;
        g_work.reset(new boost::asio::io_service::work(io_service));
        std::thread thread([&io_service]() {
                BOOST_LOG_TRIVIAL(warning)
                    << "worker thread: running io_service";
                io_service.run();
            });

        rtimer::rtimer_io_t my_timer(io_service);
        my_timer.enable(std::chrono::milliseconds(500), []() {
                BOOST_LOG_TRIVIAL(warning) << "TICK";
            });

        rtimer::rtimer_io_t my_timer2(io_service);
        my_timer2.enable(std::chrono::milliseconds(333), []() {
                BOOST_LOG_TRIVIAL(warning) << "TOCK";
            });

        thread.join();
        BOOST_LOG_TRIVIAL(warning) << "***  io_service has finished  ***";
    }

    BOOST_LOG_TRIVIAL(warning) << "Terminated";
    return 0;
}
