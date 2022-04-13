#ifndef COMMAND_H
#define COMMAND_H

#include <QObject>
#include<QMap>


class adapter
{
public:
    adapter();

    QMap<QString, int> handle_message(QString mensaje, int counter_cartasPresionadas);

    QString obtener_direccionImagen(QString mensaje);
    int obtener_comparacionCartas(QString mensaje);
    int obtener_turnoPlayer(QString mensaje);

private:

};

#endif // COMMAND_H
