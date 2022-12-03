sudo ifconfig eth0 172.17.60.19 netmask 255.255.0.0 up
sudo route add -net 172.17.0.0 netmask 255.255.0.0 dev eth0
sudo systemctl start vncserver-x11-serviced.service
cd /home/pi/Desktop/Obligatorio/venv
export FLASK_APP=app.py
export FLASK_DEBUG=1
flask run --host=0.0.0.0
