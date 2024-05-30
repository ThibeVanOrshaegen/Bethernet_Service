package main

import (
	"fmt"
	"net"
	"strings"
)

func main() {
	// Connect to the publisher
	publisherAddr := "benternet.pxl-ea-ict.be:24042"
	publisherConn, err := net.Dial("tcp", publisherAddr)
	if err != nil {
		fmt.Println("Error connecting to publisher:", err)
		return
	}
	defer publisherConn.Close()

	// Connect to the subscriber
	subscriberAddr := "benternet.pxl-ea-ict.be:24041"
	subscriberConn, err := net.Dial("tcp", subscriberAddr)
	if err != nil {
		fmt.Println("Error connecting to subscriber:", err)
		return
	}
	defer subscriberConn.Close()

	// Subscribe to a specific topic
	topic := "thibe>quest?>"
	_, err = subscriberConn.Write([]byte(topic))
	if err != nil {
		fmt.Println("Error subscribing:", err)
		return
	}

	// Receive message from publisher
	buf := make([]byte, 1024)
	n, err := publisherConn.Read(buf)
	if err != nil {
		fmt.Println("Error reading message:", err)
		return
	}
	receivedMessage := strings.TrimSpace(string(buf[:n]))

	// Check received message and send response
	response := "thibe>quest!> 13 uur"
	if receivedMessage == "thibe>quest?>tijd" {
		response = "thibe>quest!> 12 uur"
	}

	// Send response to subscriber
	_, err = subscriberConn.Write([]byte(response))
	if err != nil {
		fmt.Println("Error sending response:", err)
		return
	}

	fmt.Println("Subscribed:", receivedMessage)
}
