#include <websocketpp/config/asio_no_tls_client.hpp>
#include <websocketpp/client.hpp>
#include <websocketpp/common/thread.hpp>

#include <iostream>
#include <string>
#include <nlohmann/json.hpp>  // Include nlohmann/json

using json = nlohmann::json;
typedef websocketpp::client<websocketpp::config::asio_client> client;

void on_open(client* c, websocketpp::connection_hdl hdl) {
    std::cout << "Connected to server." << std::endl;

    // Prepare the data to send
    std::string cardName = "Ace of Spades";
    int x = 100;
    int y = 200;

    // Create a JSON object
    json jsonData;
    jsonData["cardName"] = cardName;
    jsonData["x"] = x;
    jsonData["y"] = y;

    // Convert JSON object to string
    std::string message = jsonData.dump();

    // Send the JSON string to the server
    std::cout << "Sending message: " << message << std::endl;
    c->send(hdl, message, websocketpp::frame::opcode::text);
}

void on_message(client* c, websocketpp::connection_hdl hdl, client::message_ptr msg) {
    std::cout << "Received message: " << msg->get_payload() << std::endl;
}

void on_close(client* c, websocketpp::connection_hdl hdl) {
    std::cout << "Connection closed." << std::endl;
}

int main() {
    client c;

    std::string uri = "ws://localhost:8080";

    try {
        c.set_access_channels(websocketpp::log::alevel::all);
        c.clear_access_channels(websocketpp::log::alevel::frame_payload);

        c.init_asio();

        c.set_open_handler(bind(&on_open, &c, std::placeholders::_1));
        c.set_message_handler(bind(&on_message, &c, std::placeholders::_1, std::placeholders::_2));
        c.set_close_handler(bind(&on_close, &c, std::placeholders::_1));

        websocketpp::lib::error_code ec;
        client::connection_ptr con = c.get_connection(uri, ec);
        if (ec) {
            std::cout << "Could not create connection because: " << ec.message() << std::endl;
            return 1;
        }

        c.connect(con);

        c.run();
    } catch (websocketpp::exception const & e) {
        std::cout << "WebSocket++ exception: " << e.what() << std::endl;
    } catch (std::exception const & e) {
        std::cout << "Exception: " << e.what() << std::endl;
    } catch (...) {
        std::cout << "Unknown exception" << std::endl;
    }

    return 0;
}
