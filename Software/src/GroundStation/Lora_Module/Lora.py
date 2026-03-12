from RF24 import RF24, RF24_PA_LOW

radio = RF24(22, 0)

radio.begin()
radio.setPALevel(RF24_PA_LOW)
radio.openWritingPipe(b"1Node")

print("Sending test message...")
radio.write(b"hello")
