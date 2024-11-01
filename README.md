# Bethernet_Service

The combination of ZeroMQ and Bethernet has resulted in the creation of a service designed to process financial data on stocks traded on the stock market. This documentation will examine in detail how the service functions, how data is retrieved and processed.

# Description of the service

This project employs the use of ZeroMQ and communicates over Bethernet. In order for this service to function, a client is required. This client initiates the service request and receives a response indicating the stock exchange it has requested. This service utilises the following `API: https://polygon.io/`. This ensures that the response is always updated with the most recent information. It also provide a Mastermind Service where the users can play Mastermind.

The service in question is designated the Stock Service. It provides users with information about the current state of the stock market. This includes data on the opening and closing prices. By utilising this service, investors can greatly improve their decision-making process by identifying the most suitable stocks, receiving up-to-date information and specifications about them.

# How does it work

When the client starts the service, he gets to choose which specific information he wants to know.Once the client has decided what they want to do, they just press Enter. This will send the information to the broker on the topic they're subscribed to. The broker then receives the string and starts comparing which service is expected.

Once the broker knows what information they need to request from the API, they make an HTTP request to the API. They then send the information back to the broker on the correct topic. The client can then see the answer. Once that's done, the info is parsed and displayed to the client in a better way.

![alt text](afbeelidngdiagrameen.png)

# Service

To get in touch with the service, the client just needs to subscribe to a specific topic. This way, only the client sees the answer from this specific service and not all other services that use Bethernet. To let the client know that it wants to see the Stock Exchange service, it just needs to subscribe to the topic 'thibe>quest!>StockService'. Once subscribed, the client can request a specific service from the Stock Exchange. 

The relevant stock symbol is extracted from the received request, after which HTTP requests are sent to the Polygon API to obtain the required information about the specified stock. 

# Client

To make sure the client understands this service, this section explains it from their perspective.

As a client, I have to connect to the service by using the correct addresses and ports. Outgoing messages go via `"tcp://benternet.pxl-ea-ict.be:24041" and incoming messages via ``"tcp://benternet.pxl-ea-ict.be:24042"`.
To start using the service, I send a message to request the available services. All I have to do is send a message with the text `"thibe>quest?>StockService"` and I'll receive a list of available services and instructions on how to use them.

After receiving the list of available services, I can choose the desired service and enter the required information. For example, if I want to use `"Get_detailed_information_on_shares"` and check the stock symbol `"APPL"`, I type `"thibe>quest?>Get_detailed_information_on_shares APPL"` and send this message to the service. The service will respond with the requested information, such as the opening and closing prices of the stock. In this example, the stock is `"Apple"`.

This is a great way to use the Stock Exchange Service to retrieve stock data and use it for your analysis or other purposes.

![image](https://github.com/ThibeVanOrshaegen/Bethernet_Service/assets/114075982/f95ff391-9fbe-4b56-987e-092c83f9beaa)


# Service 1: Get detailed information on shares

This service operates within the Sector of StockService and specialises in providing detailed information on specific stocks, in particular opening and closing prices. The workflow is as follows:

After receiving a request via the ZMQ service, with the prefix `"thibe>quest>?"`, Service 1 determines the client's desired outcome.

It then identifies the relevant stock symbol within the request and initiates HTTP requests to the Polygon API in order to obtain the specified information about the stocks. The API responds with a JSON object containing details about the opening and closing prices of the specified stock on the specified date. After processing, the relevant information is returned to the user via the ZMQ service.

![image](https://github.com/ThibeVanOrshaegen/Bethernet_Service/assets/114075982/27a7ffe5-1dfc-4b1d-ae6a-38d967f196d1)

![image](https://github.com/ThibeVanOrshaegen/Bethernet_Service/assets/114075982/6a66a4be-f3f7-4a42-b868-bb8c2eab7ffc)



# Service 2: Calculation of exchange rate differential

Service2 is a service that specialises in calculating the difference between the opening and closing prices of a particular stock.

The API responds with a JSON object containing details about the opening and closing prices of the specified stock.
The data obtained from the API is used by Service2 to calculate the difference between the opening and closing prices. This calculated difference is then returned to the user via the ZMQ service.

![image](https://github.com/ThibeVanOrshaegen/Bethernet_Service/assets/114075982/98579d71-5fa7-4958-ab4b-e2bca6381482)

This is what the result looks like:

![image](https://github.com/ThibeVanOrshaegen/Bethernet_Service/assets/114075982/f472873c-855e-4502-b3b2-20b8cc227c43)


# Service 3: Mastermind

Service3 is a service that users can use to play Mastermind. Mastermind is a classic board game where two players try to crack each other's secret codes. 

One player  creates a code(service) with colors, and the other player, the client has to guess the code within a certain number of turns. After each guess, the codemaker gives the codebreaker feedback.

This shows how many colors are the right colour and in the right position, and how many are the right colour but in the wrong position. The codebreaker then uses this feedback to refine their guesses until they crack the code.

This service does not use an API. There is constant communication between the client and the service. To start this service the client has to subsribed on `"thibe>quest!>"`. To start this specific service the sener has to send `"thibe>quest?>Mastermind"`. After this the serice send back that the player can start guessing. The player must send four colors and the service is going to send feedback on wich color is right and on the right place. This is an example about how the player must send the message: `"thibe>quest?>Blue Yellow Red Black"`.

![image](https://github.com/ThibeVanOrshaegen/Bethernet_Service/assets/114075982/c26c8541-9669-42a1-9d99-8f9958be57f7)

![image](https://github.com/ThibeVanOrshaegen/Bethernet_Service/assets/114075982/917e143d-f727-4f0d-bd04-a906c471b082)


