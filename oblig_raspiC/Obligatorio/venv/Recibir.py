#!/usr/bin/env python3
import serial
import time
import app as mainpy
from datetime import datetime

ser = serial.Serial('/dev/ttyAMA0',9600, timeout=1)
ser.flush()
    
   
while 1:
    if ser.in_waiting > 0:
        linea = ser.readline().decode('UTF-8').rstrip()
        linea = linea[0:len(linea)-1]
        ahora = datetime.now()
        tiempo = ("%s-%s-%s %s:%s:%s" % (ahora.year, ahora.month, ahora.day, ahora.hour, ahora.minute, ahora.second))
        with open('velSonido.txt','r') as fVel:
            velocidad = fVel.read()
        fVel.close()
        final = ("%s %s %s" % (tiempo, velocidad[0:3], linea))
        with open('Medidas.txt','a') as f:
            f.write(final)
            f.write('\n')
        f.close()
                
                
