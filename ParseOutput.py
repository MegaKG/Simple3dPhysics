import struct
import sys

H = sys.stdin.buffer.readline()

while True:
    D = sys.stdin.buffer.read(8)
    if D == b'':
        break
    Frame = struct.unpack('Q',D)[0]
    print("Frame: ",Frame)

    Bodies = struct.unpack('Q',sys.stdin.buffer.read(8))[0]
    print("Bodies: ",Bodies)

    for i in range(Bodies):
        ID = struct.unpack('Q',sys.stdin.buffer.read(8))[0]
        print("Body: ",ID)
        print("Mass: ",struct.unpack('d',sys.stdin.buffer.read(8))[0])
        print("Radius: ",struct.unpack('d',sys.stdin.buffer.read(8))[0])

        print("Position: X",struct.unpack('d',sys.stdin.buffer.read(8))[0],
        "Y",struct.unpack('d',sys.stdin.buffer.read(8))[0],
        "Z",struct.unpack('d',sys.stdin.buffer.read(8))[0],
        )

        print("Velocity: X",struct.unpack('d',sys.stdin.buffer.read(8))[0],
        "Y",struct.unpack('d',sys.stdin.buffer.read(8))[0],
        "Z",struct.unpack('d',sys.stdin.buffer.read(8))[0],
        )

        print("Kinetic Energy: ",struct.unpack('d',sys.stdin.buffer.read(8))[0])
        print("Lorentz Factor: ",struct.unpack('d',sys.stdin.buffer.read(8))[0])
    
    print("\n\n")

