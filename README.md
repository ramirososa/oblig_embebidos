# oblig_embebidos
Obligatorio Sistemas Embebidos || Manuel Navarrete y Ramiro Sosa
Hi!

Tenemos por un lado el obligatorio 1 con el codigo del PIC. 
Y por otro lado el obligatorio 2 con todo el proyecto del flask con sus archivos python, html y CSS. Y los shells que usamos de configuracion.

Ese shell se corre en un cron on boot.

Se realizo un cambio para el obligatorio 2 en el sistema embebido del PIC:
    Cuando se espera un cambio de velocidad, decibramos que utf-8 no podia codificar el siguiente caracter '\r' que es el "enter"   que esperabamos al cambio de velocidad. Ahora esperamos una 'V' al final. que enviamos a traves de la raspi cuando el usuario cambia la velocidad.
    
    
