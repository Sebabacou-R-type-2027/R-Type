# RFC: R-Type Protocol Documentation

## 1. Introduction
This document defines the structure and types of packets used in the **R-Type** protocol. This protocol is designed for efficient data transmission in a network environment, where each packet consists of a size, an identifier, a type, and data.

## 2. Structure of a Packet

Each packet is structured to include four main fields: **SIZE**, **ID**, **TYPE**, and **DATA**. These fields define the size of the contained data, the identification of the packet, its type, and the data itself.

### 2.1 Packet Format

The following table describes the structure of a packet in the **PACKET** protocol:

| **NAME** | **SIZE**         | **DESCRIPTION**                                            |
|----------|------------------|------------------------------------------------------------|
| SIZE     | 1/2/3 Bytes      | The size of the data in the DATA field. It can be encoded in 1, 2, or 3 bytes depending on the size of the data. |
| ID       | 1/2 Bytes        | Unique identifier for the packet, encoded in 1 or 2 bytes depending on the ID. |
| TYPE     | 1 Byte           | Packet type, indicating the nature of the data. Each type has a specific meaning. |
| DATA     | Variable (SIZE)  | The content of the packet, the length of which is defined by the SIZE field. |

The **SIZE**, **ID**, **TYPE**, and **DATA** fields are mandatory for every packet. The total length of the packet varies depending on the size of the data, as indicated by the **SIZE** field.

### 2.2 Size Field (SIZE)
- The **SIZE** field indicates the size of the data in the **DATA** field.
- If the size is small (less than or equal to 255 bytes), it is represented by 1 byte.
- If the size of the data is larger (up to 65535 bytes), it is represented by 2 bytes.
- If the size exceeds 65535 bytes, the field is encoded in 3 bytes (max 16777215 bytes).
> /!\ You can use a size with a size on 3 bytes, but it's not recommended, prefer to use 1 or 2 bytes and do multiple packets if needed.


### 2.3 Identification Field (ID)
- The **ID** field uniquely identifies each packet.
- This field is encoded in 1 or 2 bytes depending on the context.
- The packets sent can be distinguished by their **ID**, which is crucial for reconstructing or analyzing data over the network.

### 2.4 Type Field (TYPE)
- The **TYPE** field is a single byte that identifies the nature of the data in the packet.
- The type allows packets to be categorized for better management and interpretation on the receiver's side.

## 3. Types of Packets

The **TYPE** field in the packet differentiates between various packet types used in the protocol. Below is a list of defined packet types:

| **TYPE Value** | **Description**                                                                          |
|----------------|------------------------------------------------------------------------------------------|
| 0x00           | **PING_PACKET**: Sends a ping to a client to test connectivity or determine a host.      |
| 0x01           | **ACK_PACKET**: Used to send an acknowledgment back to the sender, can be true or false. |
| ...           | ...                                                                                      |

## 4. Packet Handling

### 4.1 Transmission
When transmitting a packet, the sender must:
1. Define the correct **SIZE** field based on the size of the data.
2. Assign a unique **ID** to the packet, ensuring that each packet can be distinctly identified.
3. Assign the **TYPE** field based on the nature of the data or request.
4. Transmit the packet, including the **DATA** field with the length corresponding to the **SIZE** field.

### 4.2 Reception
The receiver must:
1. Read the **SIZE** field to determine the length of the data.
2. Extract the **TYPE** field and interpret the data accordingly.
3. Process the **DATA** field based on the type of packet received.
4. Respond to the sender as needed, such as sending an acknowledgment or processing the data.

## 5. Error Handling
The protocol accounts for several error scenarios that may occur during packet transmission or reception:
- **Invalid Size**: If the **SIZE** field does not match the length of the received data, an **ERROR_PACKET** is returned.
- **Unknown Type**: If the **TYPE** field contains an unrecognized value, an **ERROR_PACKET** is sent with a description of the error.
- **Timeouts**: If a response to a **PING_PACKET** is not received, a reconnection attempt can be initiated, or an **ERROR_PACKET** can be generated.

## 6. Conclusion
This packet protocol provides a flexible and lightweight structure for transmitting data while ensuring the integrity of transmissions through well-defined size, type, and identification fields. The packet types allow for efficient communication management, whether for regular data, acknowledgments, or connectivity tests.
