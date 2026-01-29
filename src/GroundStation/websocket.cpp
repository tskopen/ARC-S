/*
Compile command:
clang++ -std=c++17 -o websocket websocket.cpp \
  -I/opt/homebrew/include \
  -I/opt/homebrew/opt/boost/include \
  -L/opt/homebrew/opt/boost/lib \
  -lpthread

  */

#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>


#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>
#include <boost/beast.hpp>
#include <boost/beast/ssl.hpp>

#include <cstdlib>

#include <iostream>
#include <string>

namespace http = beast::http;           // from <boost/beast/http.hpp>
namespace websocket = beast::websocket; // from <boost/beast/websocket.hpp>
namespace ssl   = net::ssl;
using namespace std;

namespace net = boost::asio;
namespace beast = boost::beast;
using namespace boost::beast;
using namespace boost::beast::websocket;

net::io_context ioc;
tcp_stream sock(ioc);
net::ssl::context ctx(net::ssl::context::tlsv12);
using tcp = boost::asio::ip::tcp;       // from <boost/asio/ip/tcp.hpp>


class WebSocketClient
{
public:
  string host;
  string port;
  string text;
WebSocketClient(string host, string port, string text) : host_(host), port_(port), ws(ioc_) {}

  string handshakeServer()
  {
    try
    {

stream<net::ssl::stream<tcp_stream>> wss(net::make_strand(ioc), ctx);
        tcp::resolver resolver{ioc};
        websocket::stream<tcp::socket> ws{ioc};

           boost::asio::ip::tcp::resolver resolver(ioc_);
           auto const results = resolver.resolve(host_, port_);
        


net::connect(ws.next_layer().next_layer(), results);
        
wss.next_layer().handshake(net::ssl::stream_base::client);
         std::cout << "Connected!" << std::endl;

ws.write(boost::asio::buffer(text_));
        
        beast::flat_buffer buffer;
        ws.read(buffer);
        string received_message = beast::buffers_to_string(buffer.data());
        ws.close(websocket::close_code::normal);

        return received_message;
    }

    catch(std::exception const& e)
    {
        return e.what();
    }

  }
private:
  string host_;
  string port_;
  string text_;
  boost::asio::io_context ioc_;
  ssl::context ctx_{ssl::context::sslv23_client};
  tcp::socket socket_{ioc_};
};

int main()
{
        WebSocketClient client("websocket.tskopen.com", "443", "test");
        
        string response = client.handshakeServer();

        std::cout << response << std::endl;

        return 0;
}

    
