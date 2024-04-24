# Bethernet_Service

The combination of ZeroMQ and Bethernet has resulted in the creation of a service designed to process financial data on stocks traded on the stock market. This documentation will examine in detail how the service functions, how data is retrieved and processed.

# Description of the service

This project employs the use of ZeroMQ and communicates over Bethernet. In order for this service to function, a client is required. This client initiates the service request and receives a response indicating the stock exchange it has requested. This service utilises the following `API: https://polygon.io/`. This ensures that the response is always updated with the most recent information.

The service in question is designated the Stock Service. It provides users with information about the current state of the stock market. This includes data on the opening and closing prices. By utilising this service, investors can greatly improve their decision-making process by identifying the most suitable stocks, receiving up-to-date information and specifications about them.

# How does it work

When the client starts the service, he gets to choose which specific information he wants to know. For example, he can choose between the current situation of a particular stock (e.g. AAPL) or all yesterday's information about that stock (e.g. PreviousAAPL). He can also request different stocks, such as AAPL (Apple) and CADL. 

Once the client has decided what they want to do, they just press Enter. This will send the information to the broker on the topic they're subscribed to. The broker then receives the string and starts comparing which service is expected.

Once the broker knows what information they need to request from the API, they make an HTTP request to the API. They then send the information back to the broker on the correct topic. The client can then see the answer. Once that's done, the info is parsed and displayed to the client in a better way.

# Service

To get in touch with the service, the client just needs to subscribe to a specific topic. This way, only the client sees the answer from this specific service and not all other services that use Bethernet. To let the client know that it wants to see the Stock Exchange service, it just needs to subscribe to the topic 'Stock>quest>'. Once subscribed, the client can request a specific service from the Stock Exchange. 

If the client wants to see a specific current stock, they just send it to the service. In this example, we'll use Apple: `Stock>quest?>AAPL`. If they want to see yesterday's stock, they just send it to the broker: `Stock>quest?>PreviousAAPL`.

The relevant stock symbol is extracted from the received request, after which HTTP requests are sent to the Polygon API to obtain the required information about the specified stock. 

# Client

To make sure the client understands this service, this section explains it from their perspective.

As a client, I have to connect to the service by using the correct addresses and ports. Outgoing messages go via `"tcp://benternet.pxl-ea-ict.be:24041" and incoming messages via ``"tcp://benternet.pxl-ea-ict.be:24042"`.

To start using the service, I send a message to request the available services. All I have to do is send a message with the text `"thibe>quest?>StockService"` and I'll receive a list of available services and instructions on how to use them.

After receiving the list of available services, I can choose the desired service and enter the required information. For example, if I want to use `"Service1"` and check the stock symbol `"APPL"`, I type `"thibe>quest?>Service1 APPL"` and send this message to the service.

The service will respond with the requested information, such as the opening and closing prices of the stock. In this example, the stock is `"Apple"`.

This is a great way to use the Stock Exchange Service to retrieve stock data and use it for your analysis or other purposes.

# Service 1: Get detailed information on shares

This service operates within the Sector of StockService and specialises in providing detailed information on specific stocks, in particular opening and closing prices. The workflow is as follows:

After receiving a request via the ZMQ service, with the prefix `"thibe>quest>?"`, Service 1 determines the client's desired outcome.

It then identifies the relevant stock symbol within the request and initiates HTTP requests to the Polygon API in order to obtain the specified information about the stocks. The API responds with a JSON object containing details about the opening and closing prices of the specified stock on the specified date. After processing, the relevant information is returned to the user via the ZMQ service.

# Service 2: Calculation of exchange rate differential

Service2 is a service that specialises in calculating the difference between the opening and closing prices of a particular stock.

The API responds with a JSON object containing details about the opening and closing prices of the specified stock.
The data obtained from the API is used by Service2 to calculate the difference between the opening and closing prices. This calculated difference is then returned to the user via the ZMQ service.
