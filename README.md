# Bethernet_Service

This repository contains a service that provides stock exchange statistics via an API.

# Description of the service

This project employs the use of ZeroMQ and communicates over Bethernet. In order for this service to function, a client is required. This client initiates the service request and receives a response indicating the stock exchange it has requested. This service utilises the following API: https://polygon.io/. This ensures that the response is always updated with the most recent information.

# How does it work

When the client starts the service, he gets to choose which specific information he wants to know. For example, he can choose between the current situation of a particular stock (e.g. AAPL) or all yesterday's information about that stock (e.g. PreviousAAPL). He can also request different stocks, such as AAPL (Apple) and CADL. Once the client has decided what they want to do, they just press Enter. This will send the information to the broker on the topic they're subscribed to. The broker then receives the string and starts comparing which service is expected.

Once the broker knows what information they need to request from the API, they make an HTTP request to the API. They then send the information back to the broker on the correct topic. The client can then see the answer. Once that's done, the info is parsed and displayed to the client in a better way.

# Server

To get in touch with the service, the client just needs to subscribe to a specific topic. This way, only the client sees the answer from this specific service and not all other services that use Bethernet. To let the client know that it wants to see the Stock Exchange service, it just needs to subscribe to the topic 'Stock>quest>'. Once subscribed, the client can request a specific service from the Stock Exchange. 

If the client wants to see a specific current stock, they just send it to the service. In this example, we'll use Apple: 'Stock>quest?>AAPL'. If they want to see yesterday's stock, they just send it to the broker: 'Stock>quest?>PreviousAAPL'.

