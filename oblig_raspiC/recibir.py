#WIP - la idea es integrar este code al app.py pero el miercoles 23 tuvimos problemas levantando los dos datos.
# En app.py agregamos un dato levantado, con una velocidad default de 340m/s, la cual se va a poder cambiar a traves de la interfaz.
import serial
import time
from datetime import datetime


if __name__ == '__main__':
    ser = serial.Serial('/dev/ttyAMA0',9600, timeout=1)
    ser.flush()

    while 1:
        linea1=''
        linea2=''
        while len(linea1)==0:
            if ser.in_waiting > 0:
                linea1 = ser.readline().decode('UTF-8').rstrip()
        print(linea1+'\n')
        while len(linea2)==0:
            if ser.in_waiting > 0:
                linea2 = ser.readline().decode('UTF-8').rstrip()
        print(linea2+'\n')
        linea2='3'+linea2
        print("aca")
        final =linea1+linea2
        with open('prueba.txt','a') as f:
            f.write(final)
            f.write('\n')
        print(final)
