package main

import (
	"fmt"
	"net"
	"strings"
	"time"
)

func main() {
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

	for {
		// Connect to the publisher
		publisherAddr := "benternet.pxl-ea-ict.be:24042"
		publisherConn, err := net.Dial("tcp", publisherAddr)
		if err != nil {
			fmt.Println("Error connecting to publisher:", err)
			// Wait before attempting to reconnect
			time.Sleep(5 * time.Second)
			continue // Retry connection
		}

		// Receive message from publisher
		buf := make([]byte, 1024)
		n, err := publisherConn.Read(buf)
		if err != nil {
			fmt.Println("Error reading message from publisher:", err)
			publisherConn.Close()
			continue // Continue listening for new messages
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
			fmt.Println("Error sending response to subscriber:", err)
			continue // Continue listening for new messages
		}

		fmt.Println("Subscribed:", receivedMessage)

		publisherConn.Close()
	}
}
