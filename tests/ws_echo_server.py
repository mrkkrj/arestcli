import asyncio
import websockets
import os

port = os.environ.get('WS_ECHO_PORT') or 8080

async def echo_loop(connection):
    print("---> client connected...")
    
    async for message in connection:
        print (">RECV: "+message, flush=True)
        await connection.send(message)
        print (">SENT: "+message, flush=True)

    print("---> client disconnected!!!")

echo_server = websockets.serve(echo_loop, "0.0.0.0", port)

print(">WS Echo Server --- starting...")
asyncio.get_event_loop().run_until_complete(echo_server)

print(">WS Echo Server --- started!!!")
asyncio.get_event_loop().run_forever()

