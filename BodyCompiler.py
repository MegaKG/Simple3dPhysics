#!/usr/bin/env python3
#This program creates input files for the physics engine
import sys
import struct
import os
ScriptName = os.path.basename(sys.argv[0])

Empty = {
    'mass':0,
    'xpos':0,
    'ypos':0,
    'zpos':0,
    'xvel':0,
    'yvel':0,
    'zvel':0,
    'radius':0,
    'grav':0,
    'charge':0,
    'allx':0,
    'ally':0,
    'allz':0
}

formats = {
    'mass':'d',
    'xpos':'d',
    'ypos':'d',
    'zpos':'d',
    'xvel':'d',
    'yvel':'d',
    'zvel':'d',
    'radius':'d',
    'grav':'c',
    'charge':'d',
    'allx':'c',
    'ally':'c',
    'allz':'c'
}

def char(IN):
    b = bytearray()
    b.append(int(IN))
    return bytes(b)

converters = {
    'mass':float,
    'xpos':float,
    'ypos':float,
    'zpos':float,
    'xvel':float,
    'yvel':float,
    'zvel':float,
    'radius':float,
    'grav':char,
    'charge':float,
    'allx':char,
    'ally':char,
    'allz':char
}

order = ['mass',
    'xpos',
    'ypos',
    'zpos',
    'xvel',
    'yvel',
    'zvel',
    'radius',
    'grav',
    'charge',
    'allx',
    'ally',
    'allz']

def main():
    #Read in the input
    BODIES = []
    Current = {}
    f = open(sys.argv[1],'r')
    while True:
        l = f.readline().replace('\t','').replace(' ','').lower()
        if l == '':
            break
        else:
            l = l.replace('\n','')
            if ('#' not in l) and (l != ''):
                if l == '[body]':
                    Current = {}
                    for key in Empty:
                        Current[key] = 0
                
                elif l == '[end]':
                    BODIES.append(Current)

                else:
                    SP = l.split('=')
                    Current[SP[0]] = SP[1]
    f.close()

    #Now Build the Output file
    o = open(sys.argv[2],"wb")
    o.write(struct.pack('Q',len(BODIES)))
    for i in BODIES:
        for a in order:
            print(formats[a],converters[a],i[a])
            o.write(struct.pack(formats[a],converters[a](i[a])))
    

    o.close()


if __name__ == '__main__':
    if len(sys.argv) == 1:
        print("Usage:\n{} <Input File> <Output File>\nEG: {} Test.phys TestBodies".format(ScriptName,ScriptName))
    else:
        print("Compiling {} to {}".format(sys.argv[1],sys.argv[2]))
        main()