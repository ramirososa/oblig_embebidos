# importing Flask and other modules
from flask import Flask, request, render_template
import serial
import time
from datetime import datetime

# Flask constructor
app = Flask(__name__)

if __name__ == '__main__':
    ser = serial.Serial(
        port='/dev/ttyAMA0', #Replace ttyS0 with ttyAM0 for Pi1,Pi2,Pi0
        baudrate = 9600,
        parity=serial.PARITY_NONE,
        stopbits=serial.STOPBITS_ONE,
        bytesize=serial.EIGHTBITS,
        timeout=1
    )
    ser.flush()


@app.route('/', methods =["GET", "POST"])

def main():
    ultima = ""
    data = ""
    h = []
    h.append("Fecha")
    h.append("Vel Sonido")
    h.append("Medida")
    velocidad = 340
    while 1:
        if ser.in_waiting > 0:
            linea = ser.readline().decode('UTF-8').rstrip()
            ahora = datetime.now()
            tiempo = ("%s-%s-%s %s:%s:%s" % (ahora.year, ahora.month, ahora.day, ahora.hour, ahora.minute, ahora.second))
            final = ("%s %s %s" % (tiempo, velocidad, linea))

            with open('prueba.txt','a') as f:
                f.write(final)
                f.write('\n')
            ultima = [tiempo, velocidad, linea]
            f.close()
        if "btn1" in request.form:
            ultima = ultima
        elif "btn2" in request.form:
            ser.write(b"x")
            time.sleep(1)
        elif "btn3" in request.form:
            #No funciona, pretendo mandar el velocidad-300, para que nos entre en un envio.
            #Habria que cambiar la recepcion del script en el pic para eso.
            velocidad = request.form.get("vel")
            ser.write(velocidad[0:1].encode('utf-8'))
            time.sleep(1)
            ser.write(velocidad[1:2].encode('utf-8'))
            time.sleep(1)
            ser.write(velocidad[2:3].encode('utf-8'))
            time.sleep(1)
            ser.write(b("/r"))
            time.sleep(1)
        elif "btn4" in request.form:
            data = desplegarMedidas(request.form.get("d"),request.form.get("h"))
        return render_template("indexardo.html", h=h, data=data, ultima=ultima)

#def ultimoDato():
#    ultimo = []
#    if request.method == "POST":
#        ultima=ultimo
#        return render_template("indexardo.html", h=h, data=data, ultima=ultima)

def desplegarMedidas(desde, hasta):
    a = []
    desdeYear = desde[0:4]
    desdeMonth = desde[5:7]
    desdeDay = desde[8:10]
    hastaYear = hasta[0:4]
    hastaMonth = hasta[5:7]
    hastaDay = hasta[8:10]
    file1 = open('Ejemplo.txt','r')
    for line in file1:
        year = line[0:4]
        month = line[5:7]
        day = line[8:10]
        if year>desdeYear and year<hastaYear:
            a.append([line[0:10],line[11:13],line[15:18]])
        elif year==desdeYear or year==hastaYear:
            if month>desdeMonth and month<hastaMonth:
                a.append([line[0:10],line[11:13],line[15:18]])
            elif month==desdeMonth or month==hastaMonth:
                if day>=desdeDay and day<=hastaDay:
                    a.append([line[0:10],line[11:13],line[15:18]])
    file1.close
    return(a)


def cortarlinea(l):
    i=0
    arr=[]
    while(l[i]!=' ')
        i++
    i++
    while(l[i]!=' ')
        i++
    i++
    arr[0]=l[0:i]
    aux = i-1
    while(l[i]!=' ')
        i++
    i++
    arr[1]=l[aux:i]
    aux=i-1
    while(l[i]!=' ')
        i++
    i++
    arr[2]=l[aux:i]
    return a


import os

with open('filename.txt', 'rb') as f:
    try:  # catch OSError in case of a one line file
        f.seek(-2, os.SEEK_END)
        while f.read(1) != b'\n':
            f.seek(-2, os.SEEK_CUR)
    except OSError:
        f.seek(0)
    last_line = f.readline().decode()
