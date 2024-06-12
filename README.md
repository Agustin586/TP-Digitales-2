# TP-Digitales-2
 Trabajos prácticos de digital 2

Tp1: Trabajo que consiste en la utilización de mef.

Tp2: La última versión consiste en la medición del acelerómetro trabajando con mef y rtos.

Tp3: Este último trabajo práctico se encarga basicamente de un radar. Tal radar fue implementado con
un rtos, en particular FreeRTOS, en donde dichas tareas poseen una mef que controla el movimiento de los datos.
Para visualizarlo utilizamos una pantalla Nextion, donde el programa se encuentra en el repositorio: https://github.com/Agustin586/Nextion_projects.
Dicha pantalla consta de Menus, es decir, se encuentra en tres estados:
     - Page Main: donde se visualizan ciertas características propias de los sensores a utilizar.
     - Page Radar: donde se tiene los datos tomados por el sensor de ultrasonido HCSR04 y donde se puede ver como
     varía el ángulo del servo motor.
     - Page Servo: si bien no funciona como esperaba, la idea es visualizar la señal pwm que se envía en cada instante
     al servo motor. También tales parámetros aparecen como datos escritos.
     - Page Easter egg: jajaja solo para boludear. Aparece benson.
Programa: el micro se encargará de detectar en que pantalla se encuentra, a través de la recepción de datos por uart1 con rtos.
Este dato es envíado por la pantalla en el momento de presionar un botón sobre esta (sendme). Como resultado de esto el programa
deberá pasar de un estado de la mefNextion a otro, con el fin de enviar datos correctos según se encuentre en una página u otra.
