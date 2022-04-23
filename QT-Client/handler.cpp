#include "handler.h"
#include "client.h"

/**
 * Constructor de la clase handler
 *
 * @brief handler::handler
 * @param interface la interfaz del cliente
 */
handler::handler(Ui::client* interface)
{
    this->interfaz = interface;

}

/**
 * Verifica el punto del juego para decidir cuales metodos llamar.
 * Retorna 1 o 2 si se debe establecer el turno del jugador, 3 o 4 si
 * es obtuvo el resultado de la comparacion de cartas, 0 si establecio
 * una imagen
 *
 * @brief handler::handle_message
 * @param mensaje
 * @param counter_cartasPresionadas
 * @return un int para que el cliente lo interprete y con eso defina aspectos graficos
 */
int handler::handle_message(QString mensaje, int counter_cartasPresionadas){

    interfaz->comparacionLabel->hide();

    dosCartas_clicked = counter_cartasPresionadas % 2 == 0 && counter_cartasPresionadas >= 2;
    ceroCartas_clicked = counter_cartasPresionadas == 0;

    if(dosCartas_clicked){
        qDebug()<<"Dos cartas"<<endl;

       set_direccionImagen(mensaje, 2);
       set_turnoPlayer(mensaje);
       return set_comparacionCartas(mensaje); // RETURN 3 o 4

    }else if(ceroCartas_clicked){ // al inicio el server envía quien empieza a jugar
        int turno = mensaje.toInt();
        return turno;  // RETURN 1 o 2

    }else{
        set_direccionImagen(mensaje, 1);
        return 0;
    }
}

void handler::set_direccionImagen(QString mensaje, int whichButton){
    //Aquí iría la lógica para parsear un byteArray a una dirección de imagen

}

/**
 * Convierte un dato QString a int
 *
 * @brief handler::set_turnoPlayer
 * @param player un QString que representa el jugador
 * @return el dato convertido en int
 */
int handler::set_turnoPlayer(QString player){

    int playerInt = player.toInt();
    return playerInt;
}

/**
 * Verifica si el mensaje corresponde a que el jugador realizo el match
 * o si no lo hizo. Retorna 3 si hizo el match, 4 si no lo hizo
 *
 * @brief handler::set_comparacionCartas
 * @param mensaje enviado por el server
 * @return un int que representa si hizo el match o no
 */
int handler::set_comparacionCartas(QString mensaje){

    interfaz->comparacionLabel->show();

    if(mensaje == "1"){

        interfaz->comparacionLabel->setText("Correcto! Cartas iguales");
        return 3;

    }else{
        qDebug()<<"Mensajeee"<<mensaje<<endl;
        interfaz->comparacionLabel->setText("Incorrecto... Cartas distintas");
        return 4;
    }
}


int handler::getFila1(){
    return fila1;
}

int handler::getFila2(){
    return fila2;
}

int handler::getColumna1(){
    return columna1;
}

int handler::getColumna2(){
    return columna2;
}

void handler::setFila1(int newFila1){
    fila1 = newFila1;
}

void handler::setFila2(int newFila2){
    fila2 = newFila2;
}

void handler::setColumna1(int newCol1){
    columna1 = newCol1;
}

void handler::setColumna2(int newCol2){
    columna2 = newCol2;
}
