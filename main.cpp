#include <iostream>
#include "list"
#include <string.h>
#include <stdlib.h>
#include <fstream>

using namespace std;

struct Casilla {
    int fila;
    int columna;
};

void AbreArch (int** & Templo,int* & AuxF,int* & AuxC,int & Fila,int & Columna,Casilla & Croft,Casilla & Cofre,int & Pasos)
{
    fstream Arch;
    Arch.open("entrada.txt" , ios::in);
    string linea;
    Arch >> linea;
    Fila = atoi(linea.substr(0,linea.find(",")).c_str());
    Columna = atoi(linea.substr((linea.find(",")+1),linea.length()).c_str());
    Templo = new int*[Fila];
    AuxF = new int[Fila];
    AuxC = new int[Columna];
    for (int i = 0;i<Fila;i++){
        Templo[i] = new int[Columna];
        AuxF[i] = 0;
    }
    for (int j=0;j<Columna;j++){
        AuxC[j] = 0;
    }
    Arch >> linea;
    Croft.fila = atoi(linea.substr(0,linea.find(",")).c_str());
    Croft.columna = atoi(linea.substr((linea.find(",")+1),linea.length()).c_str());
    Arch >> linea;
    Cofre.fila = atoi(linea.substr(0,linea.find(",")).c_str());
    Cofre.columna= atoi(linea.substr((linea.find(",")+1),linea.length()).c_str());
    for (int i =0;i<Fila;i++){
        Arch >> linea;
        for (int j=0;j<Columna;j++){
            Templo[i][j] = atoi(linea.substr(0,linea.find(",")).c_str());
            Pasos += Templo[i][j];
            AuxF[i] += Templo[i][j];
            AuxC[j] += Templo[i][j];
            linea.erase(0,linea.find(",")+1);
        }
    }
    Arch.close();
}

void GuardaResultado (list<Casilla> Camino,int estados, int resultado)
{
    fstream Arch;
    Arch.open("salida.txt",ios::out);
    Arch << estados << "\n";
    if (resultado == 1){
        typename list<Casilla>::iterator itera ;
        itera = Camino.begin();
        while (itera != Camino.end()){
            Arch << "(" << itera->fila << "," << itera->columna << ") \n";
            itera++;
        }
    }
    if (resultado == -1){
        Arch << resultado << "\n";
    }
    Arch.close();
}

bool Tesoro (Casilla Croft, Casilla Meta)
{
    if (( Croft.fila == Meta.fila )&&( Croft.columna == Meta.columna )){
        return true;
    }
    return false;
}

bool MovFactible (int** matriz,int fila, int columna, int maxf, int maxc)
{
    if ( (fila >=0) && (columna >= 0) ){
        if ((fila < maxf) && (columna < maxc) && (matriz[fila][columna] != 0) ){
                return true;
        }
    }
    return false;
}

bool Acorta(int* arr, int pos, int & fin)
{
    if (arr[pos] == 1){
        int DerAux = -1,IzqAux = -1;
        for (int i=pos;i<fin;i++){
            DerAux += arr[i];
        }
        for (int j=pos;j>=0;j--){
            IzqAux += arr[j];
        }
        if ((DerAux >0) && (IzqAux >0)){
            return true;
        }
    }
    return false;
}
bool Poda(int* AuxF, int* AuxC, Casilla Croft, Casilla Movi, int MaxF, int MaxC) {
    int NuevaF = Croft.fila + Movi.fila;
    int NuevaC = Croft.columna + Movi.columna;

    if (Movi.fila != 0 && Movi.columna == 0) {
        // Comprueba si la fila a la que nos moveríamos se puede acortar
        return Acorta(AuxF, NuevaF, MaxF);
    }

    if (Movi.columna != 0 && Movi.fila == 0) {
        // Comprueba si la columna a la que nos moveríamos se puede acortar
        return Acorta(AuxC, NuevaC, MaxC);
    }

    if (Movi.fila != 0 && Movi.columna != 0) {
        // Comprueba si ambas filas y columnas se pueden acortar
        return Acorta(AuxF, NuevaF, MaxF) && Acorta(AuxC, NuevaC, MaxC);
    }

    return false;
}

/*
bool Poda(int* AuxF,int* AuxC,Casilla Croft,Casilla Movi, int MaxF,int MaxC)
{
    if ((Movi.fila != 0) && (Movi.columna == 0) ){
        return Acorta(AuxF,Croft.fila+Movi.fila,MaxF);
    }
    if ((Movi.columna != 0) && (Movi.fila == 0) ){
        return Acorta(AuxC,Croft.columna+Movi.columna,MaxC);
    }
    if ((Movi.fila != 0) && (Movi.columna != 0) ){
        return (( Acorta(AuxF,Croft.fila+Movi.fila,MaxF)) && (Acorta(AuxC,Croft.columna+Movi.columna,MaxC)));
    }

}
 */
void Escape (int** Cuarto, int* Auxf, int* Auxc, Casilla Moves[], Casilla & Croft, Casilla Salida, int FinF, int FinC, int & estados, int Mov, int & Solve ,list<Casilla> & Camino,bool & seguir)
{
    estados++;
    if ( Cuarto[Croft.fila][Croft.columna] != 0){
        if (seguir){
        Mov--;
        Cuarto[Croft.fila][Croft.columna]--;
        Auxf[Croft.fila]--;
        Auxc[Croft.columna]--;
        Camino.push_back(Croft);
        if(Mov == 0){
            if (Tesoro(Croft, Salida)){
                Solve = 1;
                seguir = false;
            }
        }else{
            for (int i=0;i<6;i++){
                int NuevaF = Croft.fila + Moves[i].fila;
                int NuevaC = Croft.columna + Moves[i].columna;
                if ( (MovFactible(Cuarto,NuevaF,NuevaC,FinF,FinC))  && (!Poda(Auxf,Auxc,Croft,Moves[i],FinF,FinC)) ){
                    Croft.fila += Moves[i].fila;
                    Croft.columna += Moves[i].columna;
                    Escape(Cuarto,Auxf,Auxc,Moves,Croft,Salida,FinF,FinC,estados,Mov,Solve,Camino,seguir);
                    Croft.fila -= Moves[i].fila;
                    Croft.columna -= Moves[i].columna;
                }
            }

        }
        if (seguir){
            Mov++;
            Cuarto[Croft.fila][Croft.columna]++;
            Auxf[Croft.fila]++;
            Auxc[Croft.columna]++;
            Camino.pop_back();
        }
    }
}
}

void AgregaMoves ( Casilla Movimientos[])
{
    ///arriba
    Movimientos[5].fila = -1;
    Movimientos[5].columna = 0;
    ///derecha
    Movimientos[0].fila = 0;
    Movimientos[0].columna = 1;
    ///abajo
    Movimientos[1].fila = 1;
    Movimientos[1].columna = 0;
    ///izquierda
    Movimientos[3].fila = 0;
    Movimientos[3].columna = -1;
    ///diagonal inferior izquierda
    Movimientos[4].fila = 1;
    Movimientos[4].columna = -1;
    ///diagonal superior derecha
    Movimientos[2].fila = -1;
    Movimientos[2].columna = 1;
}

void MuestraCamino (list<Casilla> Camino)
{
    typename list<Casilla>::iterator itera;
    itera = Camino.begin();
    while (itera != Camino.end()){
        cout << "(" << itera->fila << "," << itera->columna << ")\n";
        itera++;
    }
}

void ImprimeTemplo (int** matriz, int maxf, int maxc)
{
    for (int i=0;i<maxf;i++){
        for(int j=0;j<maxc;j++){
            cout << matriz[i][j] << ",";
        }
        cout << "\n";
    }
}

int main()
{
    int** matriz;
    int* AuxFila;
    int* AuxColumna;
    Casilla Moves[6];
    Casilla Indiana,Salida;
    list<Casilla> Camino;
    int estados=0,solucion = -1;
    int MaxF,MaxC,movimientos =0;
    bool seguir = true;
    AgregaMoves(Moves);
    AbreArch(matriz,AuxFila,AuxColumna,MaxF,MaxC,Indiana,Salida,movimientos);
    cout << "Sala: \n";
    ImprimeTemplo(matriz,MaxF,MaxC);
    Escape(matriz,AuxFila,AuxColumna,Moves,Indiana,Salida,MaxF,MaxC,estados,movimientos,solucion,Camino,seguir);
    GuardaResultado(Camino,estados,solucion);
    if (solucion == 1){
        cout << "Solucion \n";
        MuestraCamino(Camino);
        cout << "Estados: " << estados << "\n";
    }else
        cout << "No hay solucion \n";
    return 0;
}

