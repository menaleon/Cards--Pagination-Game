#include "handler.h"
#include "client.h"

handler::handler(Ui::client* interface)
{
    this->interfaz = interface;

}

int handler::handle_message(QString mensaje, int counter_cartasPresionadas){

    interfaz->comparacionLabel->hide();

    dosCartas_clicked = counter_cartasPresionadas %2 == 0 && counter_cartasPresionadas >= 2;
    ceroCartas_clicked = counter_cartasPresionadas == 0;

    if(dosCartas_clicked){

       set_direccionImagen(mensaje, 2);

       return set_comparacionCartas(mensaje); // RETURN 3 o 4

    }else if(ceroCartas_clicked){ // al inicio el server envía quien empieza a jugar

        return set_turnoPlayer(mensaje); // RETURN 1 o 2

    }else{

        set_direccionImagen(mensaje, 1);

        return 0;
    }
}

void handler::set_direccionImagen(QString mensaje, int whichButton){
    //Aquí iría la lógica para parsear un byteArray a una dirección de imagen

}

int handler::set_turnoPlayer(QString player){

    int playerInt = player.toInt();
    return playerInt;
}

int handler::set_comparacionCartas(QString mensaje){

    interfaz->comparacionLabel->show();

    if(mensaje == "1"){

        interfaz->comparacionLabel->setText("Correcto! Cartas iguales");
        return 3;

    }else{

        interfaz->comparacionLabel->setText("Incorrecto... Cartas distintas");
        //set_botonEnMatriz(4);
        return 4;
    }
}


