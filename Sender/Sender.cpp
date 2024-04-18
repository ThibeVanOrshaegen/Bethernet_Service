#include <iostream>
#include <string>
#include <zmq.hpp>
#ifndef _WIN32
#include <unistd.h>
#else
#include <windows.h>
#define sleep(n)    Sleep(n)
#endif

int main( void )
{
    try
    {
        zmq::context_t context(1);

        //Outgoing message go out through here
        zmq::socket_t ventilator( context, ZMQ_PUSH );
        ventilator.connect( "tcp://benternet.pxl-ea-ict.be:24041" );

        //Incoming messages come in here
        zmq::socket_t subscriber( context, ZMQ_SUB );
        subscriber.connect( "tcp://benternet.pxl-ea-ict.be:24042" );
        subscriber.setsockopt( ZMQ_SUBSCRIBE, "Stock>quest?>", 13 );

        zmq::message_t * msg = new zmq::message_t();

        ventilator.send(  "Stock>quest?>AAPL", 17 );
        subscriber.recv( msg );

        std::cout << "Subscribed : [" << std::string( (char*) msg->data(), msg->size() ) << "]" << std::endl;

    }
    catch( zmq::error_t & ex )
    {
        std::cerr << "Caught an exception : " << ex.what();
    }

    return 0;
}
//https://api.polygon.io/v1/open-close/CADL/2023-01-09?adjusted=true&apiKey=9R_AeMLkHBOPzgqDs2HiizXhCI4r67CS
