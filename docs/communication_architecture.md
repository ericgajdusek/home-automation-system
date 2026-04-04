## Overview

This system uses an ACK regulated communication system with a standardized message structure throughout all devices. Although primarily using MQTT, this system supplements other communication protocols for specific purposes. 

## Protocol

Technologies used:
- MQTT
- ESP-NOW

During normal operation, MQTT is used for communication between all local devices. The server serves as the MQTT broker and all ESP32 nodes serve as MQTT clients, subscribing to the channels related to their purpose.  

During the WiFi set-up stage, since none of the nodes are connected to a network and cannot use MQTT, ESP-NOW is used to securely communicate network credentials to all ESP32 nodes from the controller. This connection is then verified using MQTT.
## Message structure

Each message is a JSON object with parameters depending on the **message_type**. 

| Field          | Type   | Required                                         | Description                                                 | Examples                                      |
| -------------- | ------ | ------------------------------------------------ | ----------------------------------------------------------- | --------------------------------------------- |
| target         | String | Always                                           | Destination node                                            | "SERVER", "LIGHTS", "BLINDS", "CONTROLLER"    |
| message_id     | String | Always                                           | Unique identifier for each message instance                 | "m_123"                                       |
| transaction_id | String | Always                                           | Identifier linking related messages in the same transaction | "t_120"                                       |
| message_type   | String | Always                                           | Specifies message purpose                                   | "COMMAND", "ACK", "STATUS", "ERROR"           |
| action         | String | message_type =  "COMMAND" or "STATUS" or "ERROR" | Action being performed in this transaction                  | "LIGHTS_SET", "BLINDS_SET", "SPEAK", "LISTEN" |
| payload        | Object | message_type =  "COMMAND" or "STATUS" or "ERROR" | Data associated with action                                 | Shown below                                   |
| sender         | String | Always                                           | Origination node                                            | Same as target                                |
| timestamp      | int    | Always                                           | Unix timestamp in seconds                                   | 1775185955                                    |
| retry_count    | int    | Always                                           | Number of times message was resent                          | Default: 0                                    |
##### Payload Structure

The **payload** field of the message is its own object that contains data specific to the type of message being delivered and the action the message is intended to perform. 

| Field             | Type   | Required                                                      | Description                               |
| ----------------- | ------ | ------------------------------------------------------------- | ----------------------------------------- |
| lights_brightness | int    | message_type = "COMMAND" or "STATUS"<br>action = "LIGHTS_SET" | Brightness level for lights node (0-100)  |
| blinds_position   | int    | message_type = "COMMAND" or "STATUS"<br>action = "BLINDS_SET" | Position for blinds node (0-100)          |
| error_code        | int    | message_type = "ERROR"                                        | Integer specifying type of error received |
| error_message     | String | message_type = "ERROR"                                        | Description of error received             |

## Flow

In this design, the controller sends user commands to the server, such as changing light brightness or moving the blinds to a target position. The server acknowledges receipt of the command, records the transaction, and forwards the command to the appropriate node. After the target node performs the action, it sends a completion or error message back to the server. The server then updates the stored system state and sends the final result back to the controller so the display reflects the confirmed state of the system.

Here is a very simple animation giving a rough visual of how transactions flow through the system. In this case, the user is setting the lights level from 0% to 100%. [Link to full video.](https://youtu.be/N4yrDLcrTSg)

![Communication Flow](./docs/assets/comm_flow.gif)

## ACK-based messaging

To improve reliability, the system uses ACK-based messaging. Each command is associated with a message ID and transaction ID so that acknowledgments, status updates, retries, and errors can be matched to the correct operation. The communication protocol distinguishes between receipt of a message and completion of an action. This allows the server to track whether a command was received, whether it was executed successfully, and whether the controller display should be updated.