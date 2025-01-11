import asyncio
import websockets
import sys  

def process_lines(lines):
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
    return results

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
                    print(message)
                    output_list = [line for line in message.strip().split("\n")]
                    # print(output_list)
                    output = process_lines(output_list)
                    for line in output:
                        print(line)
                except asyncio.TimeoutError:
                    print(f"No exchange for {no_exchange_timeout} seconds. Stopping the program.")
                    sys.exit(0)  # Exit the program
    except Exception as e:
        print(f"Connection lost: {e}. Stopping the program.")
        sys.exit(0)  # Exit the program

if __name__ == "__main__":
    asyncio.run(receive_message())
