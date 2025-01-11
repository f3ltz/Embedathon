import asyncio
import websockets
import sys  
import numpy as np
import json

MAXSIZE = 20

def process_lines(lines):
    """
    Processes a list of strings where each string contains digits.
    Alternates between creating '*' and ' ' based on the digits and outputs the result.

    :param lines: List of strings containing digits
    :return: List of processed strings
    """
    results = []
    
    for index, raw in enumerate(lines):
        result = []
        tot = 0
        space_or_star = 0  # 0 for '*', 1 for ' '
        
        for ch in raw:
            count = int(ch)  # Convert character to integer
            tot += count
            if space_or_star == 0:
                result.append('1' * count)  # Add count number of '*'
                space_or_star = 1  # Switch to ' '
            else:
                result.append('0' * count)  # Add count number of ' '
                space_or_star = 0  # Switch to '*'

        # print(tot)
        ans = 7 - tot % 7
        lines[index] = lines[index] + str(ans)
        result.append('0' * ans)
        results.append("".join(result))
    results = [[int(char) for char in row] for row in results]
    # print(lines)
    return results

def revandtransform(results):
    """
    Splits the results into blocks and reverses the order of the blocks.
    
    :param results: 2D list (or array) of processed strings.
    :return: List of reversed blocks
    """
    results = np.array(results)
    nblocks = len(results[0]) // 7  # Integer division for the number of blocks
    blocks = [results[:, i * 7:(i + 1) * 7] for i in range(nblocks)]  # Split into blocks
    reversed_blocks = blocks[::-1]  # Reverse the order of blocks
    reversed_blocks = [np.pad(matrix, pad_width=1, mode='constant', constant_values=0) for matrix in reversed_blocks]
    a = np.zeros((8,8), dtype=int)
    for n in range(MAXSIZE-nblocks):
        reversed_blocks.append(a) 
    
    return reversed_blocks,nblocks

def print_lines(lines):
    """
    Processes a list of strings where each string contains digits.
    Alternates between creating '*' and ' ' based on the digits and outputs the result.
    
    :param lines: List of strings containing digits
    :return: List of processed strings
    """
    results = []
    for raw in lines:
        result = []
        space_or_star = 0  # 0 for '*', 1 for ' '
        for ch in raw:
            count = int(ch)  # Convert character to integer
            if space_or_star == 0:
                result.append('*' * count)  # Add count number of '*'
                space_or_star = 1  # Switch to ' '
            else:
                result.append(' ' * count)  # Add count number of ' '
                space_or_star = 0  # Switch to '*'
        results.append("".join(result))
    
    for line in results:
        print(line)
    return None

async def receive_message():
    uri = "ws://192.168.114.119:81"
    no_exchange_timeout = 30  # Time in seconds to wait for data before stopping

    try:
        async with websockets.connect(uri, ping_interval=10, ping_timeout=5) as websocket:
            print("Connected to ESP32 WebSocket server.")
            while True:
                try:
                    # Wait for a message with a timeout
                    message = await asyncio.wait_for(websocket.recv(), timeout=no_exchange_timeout)
                    print("Message received from ESP32:")
                    
                    message = [line for line in message.strip().split("\n")]
                    
                    print_lines(message)

                    out = process_lines(message)
                    transformed,length = revandtransform(out)
                    return transformed , length
                except asyncio.TimeoutError:
                    print(f"No exchange for {no_exchange_timeout} seconds. Stopping the program.")
                    sys.exit(0)  # Exit the program
    except Exception as e:
        print(f"Connection lost: {e}. Stopping the program.")
        sys.exit(0)  # Exit the program

async def send_matrices(matrices,length):
    uri = "ws://192.168.114.119:81"  # Replace with your ESP32 WebSocket URL
    
    # Example matrices
   
    
    # Convert matrices to a list of lists (for JSON serialization)
    matrices_list = [matrix.tolist() for matrix in matrices]
    
    data_to_send = {
        'matrices': matrices_list,
        'integer_value': length
    }
    # Convert to JSON string
    data_json = json.dumps(data_to_send)
    

    # Send the data over WebSocket
    async with websockets.connect(uri) as websocket:
        await websocket.send(data_json)


if __name__ == "__main__":

    processedmsg, length = asyncio.run(receive_message())
   
    #print(processedmsg)
    asyncio.run(send_matrices(processedmsg,length ))
    


