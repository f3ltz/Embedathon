## Milestone 2: How we sent to and decrypted the message on another PC

### Step 1. (Sort of) Manual Decryption

From the Problem Statement Document:
```
A mysterious sequence of digits, such as 13125216175, has
been transmitted by the ESP32. It appears to encode a hidden
visual pattern.
The sequence alternates between two roles, guiding the
structure of the pattern.
Your task is to interpret these numbers and reconstruct the
encoded representation.
```

- `visual pattern` indicated that there was definitely use of plotting techniques 
- `The sequence alternates between two roles, guiding the
structure of the pattern.` This meant we needed to alternate between the two symbols here we took as `*` and ` `
- For the even index we used `*` and ` ` for odd index
- We used the pattern for each line and got the outut ***LAKHTARUS*** which is the reverse of **SURATHKAL**
- So now we know to expect LAKHTARUS on the receiving computer.

### Step 2. Actually sending the data to the second PC
#### a. ESP32
- On the ESP we have a websocket running which takes the Key values from NVS of the ESP and converts it to readable format using the Preferences library.
- It then sends the data to the computer which decrypts it.
- **[This](final_task/milestone2/transmit_password_websocket/transmit_password_websocket.ino)** is the code which does this.

#### b. Receiving PC
- On the Receiving PC we have a python script which has 2 main functionalities.
- First part of the script deals with receiving the data from the websocket. Once it's received it, it stores it in a string and prints out that string for verification purposes.
- Second part of the script deals with decrypting the data received from the websocket. Once we have stored the received data in a string, we parse through it and print `*` and ` ` according to the rules established in manual decryption part.
- Upon performing this action, this is the output received.

#### c. Running the code
**[This](/final_task/milestone2/receive.py)** is the code which performs all this. To run this code follow the following steps:

  1. Change the ssid and password in transmit_password_websocket to the ssid and password of your network.
  2. Upload the code in transmit_password_websocket onto your ESP32.
  3. Press the reset button on your ESP32
  4. Clone the repo to a location of your choice on the receiving pc and change directory to that location
  5. Run `/final_task/milestone2/receive.py`