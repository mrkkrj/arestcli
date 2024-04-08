import asyncio
import pathlib
import ssl
import websockets
import os

port = os.environ.get('WSS_ECHO_PORT') or 8081

async def echo_loop(connection):
    print("--->  SSL client connected...")
    
    async for message in connection:
        print (">RECV: "+message, flush=True)
        await connection.send(message)
        print (">SENT: "+message, flush=True)

    print("---> SSL client disconnected!!!")

ssl_context = ssl.SSLContext(ssl.PROTOCOL_TLS_SERVER)
localhost_pem = pathlib.Path(__file__).with_name("localhost.pem")
ssl_context.load_cert_chain(localhost_pem)

ssl_echo_server = websockets.serve(echo_loop, "0.0.0.0", port, ssl=ssl_context)

print(">WSS Echo Server --- starting...")
asyncio.get_event_loop().run_until_complete(ssl_echo_server)

print(">WSS Echo Server --- started!!!")
asyncio.get_event_loop().run_forever()

