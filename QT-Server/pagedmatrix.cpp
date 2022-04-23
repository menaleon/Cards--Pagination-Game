#include "pagedmatrix.h"


/**
 * Constructor de la clase pagedMatrix
 *
 * @brief PagedMatrix::PagedMatrix
 */
PagedMatrix::PagedMatrix()
{

}

/**
 * Saca una carta e introduce una nueva en el vector de memoria, dependiendo del
 * indice con el que se almaceno en disco
 *
 * @brief PagedMatrix::cargar_carta
 * @param fila de la carta
 * @param col de la carta
 * @return true, si logra cargar la carta en memoria
 */
bool PagedMatrix::cargar_carta(int fila, int col){
    qDebug()<<"ondisc size "<<onDiscCards.size()<<endl;

    for(size_t i = 0; i<onDiscCards.size(); i++){ //lo hago en un for porque no siempre serán 12 cartas en inMemory
        if(onDiscCards.at(i).carta.row == fila && onDiscCards.at(i).carta.column == col){
            qDebug()<<"inMemory size "<<inMemoryCards.size()<<endl;
           inMemoryCards.pop_back();
           inMemoryCards.push_back(onDiscCards.at(i)); // carga la tarjeta a inMemory
           pageFaults++;
           return true;
        }
    }

    return false;
}

/**
 * Se encarga de verificar si una carta ya esta cargada en memoria. En caso
 * de que no lo este, la manda a cargar y devuelve su index
 *
 * @brief PagedMatrix::buscar_CartasCargadas
 * @param fila de la carta
 * @param col de la carta
 * @return el indice de la carta (size_t), o bien numeros 100 o 50 para indicar alguna eventualidad
 */
size_t PagedMatrix::buscar_CartasCargadas(int fila, int col){

    bool isLoaded = false;

    if(fila == -1 && col == -1){ //solo me piden buscar una carta
        qDebug()<<"solo me pidieron una carta"<<endl;
        return 50;
    }


    for(size_t i = 0; i<inMemoryCards.size(); i++){
        if(inMemoryCards.at(i).carta.row == fila && inMemoryCards.at(i).carta.column == col){
            isLoaded = true; // si encuentra la carta entonces sí estaba cargada en memoria
            pageHits++;
            return i;
        }
    }

    if( !isLoaded){ // si no está cargada, la carga y devuelve su index
        cargar_carta(fila, col);
        for(size_t i = 0; i<inMemoryCards.size(); i++){
            if(inMemoryCards.at(i).carta.row == fila && inMemoryCards.at(i).carta.column == col){
                return i;
            }
        }
    }
    return 100; // la carta no está cargada
}

/**
 * Genera n numeros enteros del 0 al 35 de forma aleatoria, en donde n depende del
 * parametro vectorSize
 *
 * @brief PagedMatrix::random_int
 * @param vectorSize la cantidad de numeros aleatorios deseados
 * @return un vector que contiene los numeros aleatorios generados
 */
vector<int> PagedMatrix::random_int(size_t vectorSize){
    int num;
    vector<int> randIndex;

    randIndex.clear();

    for(size_t cont = 0; cont < vectorSize; cont++){
        num = 0 + rand()%(35 + 1 - 0);
        randIndex.push_back(num);
    }

    return randIndex;
}

/**
 * Lee el archivo binario que contiene las tarjetas y las copia en la estructura onDiscCards
 * Esto es util para luego mostrar dichas tarjetas en consola
 *
 * @brief PagedMatrix::leer_arrayArchivo
 * @return la estructura que contiene las tarjetas
 */
vector<Card> PagedMatrix::leer_arrayArchivo(){
    Carta temp[36];
    disco.open("cartas2.bin", ios::in | ios::binary);
    disco.read(reinterpret_cast<char*>(temp), 36* sizeof(Carta));
    disco.close();

    onDiscCards.clear();
    inMemoryCards.clear();

    for(Carta& c : temp){
        Card* temporal = new Card(c.row, c.column, c.type, c.ganada);
        onDiscCards.push_back(*temporal);
        free(temporal);
    }
    return onDiscCards;
}

/**
 * Manda a crear numeros random y los inserta en la estructura inMemory (memoria disponible).
 * La cantidad de elementos en memoria depende de cuantas cartas ya tienen match, es decir
 * aqui se recorta la memoria disponible.
 *
 * @brief PagedMatrix::llenar_inMemory
 */
void PagedMatrix::llenar_inMemory(){ // al iniciar el juego lleno el vector con 12 cartas random las cuales leo desde el disco
    vector<int> randIndexes;
    int ind;

    randIndexes.clear();
    inMemoryCards.clear();

    if(cont_llenadoInMemory == 0){
         randIndexes = random_int(12);
         cont_llenadoInMemory++;

    }else{
        cont_llenadoInMemory++;
        size_t useful_size = verificar_ganadas();
        randIndexes = random_int(((36-useful_size)/3) + 1);
    }


    for(size_t cont= 0; cont<randIndexes.size(); cont++){
        ind = randIndexes.at(cont); // me devuelve numeros que fueron creados aleatoriamente
        inMemoryCards.push_back(onDiscCards.at(ind)); //carga esas tarjetas en memoria
    }
}

/**
 * Si una carta tiene en "true" la propiedad de ganada significa que ya tiene
 * match, lo cual en este metodo permite saber cuantas cartas descontar del total
 * propuesto inicialmente.
 *
 * @brief PagedMatrix::verificar_ganadas
 * @return una variable que representa el numero de cartas con match (ganadas)
 */
size_t PagedMatrix::verificar_ganadas(){

    size_t checkedSize = 0;

    for(size_t cont = 0; cont <onDiscCards.size(); cont++){
        if(onDiscCards.at(cont).carta.ganada == true){
            checkedSize++;
        }
    }
    return checkedSize;
}

/**
 * Modifica el archivo binario y escribe en el un array de structs Carta
 *
 * @brief PagedMatrix::escribir_archivo
 * @param arrayCartas
 */
void PagedMatrix::escribir_archivo(Carta arrayCartas[36]){
    disco.open("cartas2.bin", ios::binary | ios::out);
    disco.write(reinterpret_cast<char*>(arrayCartas), 36 * sizeof (Carta));
    disco.close();
    qDebug()<<"-------El archivo binario se modificó-------"<<endl;
}

/**
 * Inicializa un array de 36 struct cartas
 *
 * @brief PagedMatrix::llenar_array
 */
void PagedMatrix::llenar_array(){ //esto es para definir las cartas que usaremos. debo lidiar con los types

    int index = 0;

    for(int col = 0; col<6; col++){
        for(int fila = 0; fila<6; fila++){
            allCards[index] = {fila,col,0,false};
            index++;
        }
    }
    escribir_archivo(allCards);
}

/**
 * Modifica el array y manda a escribir los nuevos datos en el archivo
 * binario creado anteriormente.
 *
 * @brief PagedMatrix::update_array
 * @param f1
 * @param c1
 * @param f2
 * @param c2
 */
void PagedMatrix::update_array(int f1, int c1, int f2, int c2){

    for(int i = 0; i<36; i++){
        if(allCards[i].row == f1 && allCards[i].column == c1){
            allCards[i].ganada = true;
        }

        if(allCards[i].row == f2 && allCards[i].column == c2){
            allCards[i].ganada = true;
        }
    }
    escribir_archivo(allCards);
    leer_arrayArchivo();
    llenar_inMemory();
}
