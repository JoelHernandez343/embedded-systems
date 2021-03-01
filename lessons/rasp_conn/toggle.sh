#!/bin/bash

GPIO=16

export_gpio(){
    if [ ! -d /sys/class/gpio/gpio$GPIO ]
    then
        # export 
        echo $GPIO > /sys/class/gpio/export
        sleep 1
    fi
}

unexport_gpio(){
    # Comprobando si existe el directorio
    if [ ! -d /sys/class/gpio/gpio$GPIO ]
    then
        # unexport
        echo $GPIO > /sys/class/gpio/unexport
    fi
}

export_gpio

# Configuramos la dirección como entrada(in) o salida (out)
echo "out" > /sys/class/gpio$GPIO/direction

# Escribimos un valor en el puerto
while [ 1 ]
do
    echo "1" > /sys/class/gpio$GPIO/value
    sleep 1
    echo "0" > /sys/class/gpio$GPIO/value
    sleep 1
done

unexport_gpio

exit 0