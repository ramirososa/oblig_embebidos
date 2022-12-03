# importing Flask and other modules
from flask import Flask, request, render_template
import serial
import time
from datetime import datetime
import os

# Flask constructor
app = Flask(__name__)

ser = serial.Serial('/dev/ttyAMA0',9600, timeout=1)
ser.flush()


h=""
data=""
ultima=""

def cortarlinea(l):
    i=0
    arr=['','','']
    while l[i]!=' ':
        i=i+1
    i=i+1
    while l[i]!=' ':
        i=i+1
    arr[0]=l[0:i]
    aux = i+1
    i=i+1
    while l[i]!=' ':
        i=i+1
    arr[1]=l[aux:i]
    aux = i+1
    arr[2]=l[aux:len(l)]
    return arr




@app.route('/', methods =["GET", "POST"])

def formulario():
    ultima=""
    data = ""
    h = []
    h.append("Fecha")
    h.append("Vel Sonido")
    h.append("Medida")
    if "btn1" in request.form:
        with open("Medidas.txt", "rb") as file:
            file.seek(-2, os.SEEK_END)
            while file.read(1) != b'\n':
                file.seek(-2, os.SEEK_CUR)
            ultima = cortarlinea(file.readline().decode('utf-8'))
    elif "btn2" in request.form:
        ser.write(("x").encode('utf-8'))
        time.sleep(1)
    elif "btn3" in request.form:
        #No funciona, pretendo mandar el velocidad-300, para que nos entre en un envio.
        #Habria que cambiar la recepcion del script en el pic para eso.
        velocidad = request.form['vel']
        with open('velSonido.txt','w') as f:
            f.write(velocidad)
            f.write('\n')
        f.close()
        ser.write(velocidad[0:1].encode('utf-8'))
        time.sleep(1)
        ser.write(velocidad[1:2].encode('utf-8'))
        time.sleep(1)
        ser.write(velocidad[2:3].encode('utf-8'))
        time.sleep(1)
        ser.write(b"V")
        time.sleep(1)
    elif "btn4" in request.form:
        data = desplegarMedidas(request.form.get("d"),request.form.get("h"))
    return render_template("indexardo.html", h=h, data=data, ultima=ultima)


def desplegarMedidas(desde, hasta):
    a = []
    desdeYear = desde[0:4]
    desdeMonth = desde[5:7]
    desdeDay = desde[8:10]
    hastaYear = hasta[0:4]
    hastaMonth = hasta[5:7]
    hastaDay = hasta[8:10]
    file1 = open('Medidas.txt','r')
    for line in file1:
        year = line[0:4]
        month = line[5:7]
        day = line[8:10]
        if year>desdeYear and year<hastaYear:
            a.append(cortarlinea(line))
        elif year==desdeYear or year==hastaYear:
            if month>desdeMonth and month<hastaMonth:
                a.append(cortarlinea(line))
            elif month==desdeMonth or month==hastaMonth:
                if day>=desdeDay and day<=hastaDay:
                    a.append(cortarlinea(line))
    file1.close
    return(a)


