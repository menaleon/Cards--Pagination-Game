#include "adapter.h"


adapter::adapter()
{

}

QMap<QString, int> adapter::handle_message(QString mensaje, int counter_cartasPresionadas){

    QMap<QString, int>* response_Handled = new QMap<QString, int>;

    QString direccionImagen = obtener_direccionImagen(mensaje);

    if(counter_cartasPresionadas %2 == 0){

        int result_comparacionCartas = obtener_comparacionCartas(mensaje);
        response_Handled->insert(direccionImagen, result_comparacionCartas);

    }else{
        response_Handled->insert(direccionImagen, -1);
    }

    return *response_Handled;
}

QString adapter::obtener_direccionImagen(QString mensaje){
    //Aquí iría la lógica para parsear un byteArray a una dirección de imagen
    return ":D";
}

int adapter::obtener_turnoPlayer(QString mensaje){

    int* turno = (int*) &mensaje;
    return *turno;
}

int adapter::obtener_comparacionCartas(QString mensaje){

    int* resultado = (int*) &mensaje;

    return *resultado;
}

