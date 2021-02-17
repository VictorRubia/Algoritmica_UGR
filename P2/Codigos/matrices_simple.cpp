#include<iostream>
#include<chrono>
#include<math.h>
#include<fstream>
#include<cstdlib>

using namespace std;

int main(int argc, char *argv[]){

    int n, i, argumentos;
    int **M, **N;
    int fil, col;
    chrono::time_point<std::chrono::high_resolution_clock> t0, tf;  //  Para medir el tiempo de ejecución
    double t_ejec; //   Tiempo de ejecución algoritmo en ms
    unsigned long int sem;
    ofstream salida;
    ofstream salidamedias;

    if(argc <= 3) {
        cerr << "\nError: El programa se debe ejecutar de la siguiente forma.\n\n";
        cerr << argv[0] << "nombre_fichero_salida semilla tamcaso1 tamcaso2 ... tamcasoN\n\n";
        return 0;
    }

    //  Abrimos fichero de salida
    salida.open(argv[1]);
    if (!salida.is_open()){
        cerr << "Error: No se pudo abrir fichero para escritura " << argv[1] << "\n\n";
        return 0;
    }

    //  Abrimos fichero de cálculo de medias
    salidamedias.open("Media_Matrices.txt");
    if (!salidamedias.is_open()) {
        cerr << "Error: No se pudo abrir fichero para escritura Media_Matrices" << "\n\n";
        return 0;
    }

    //  Generador de números aleatorios
    sem = atoi(argv[2]);
    srand(sem);

    //  Pasamos por cada tamaño de caso
    for (argumentos = 3; argumentos < argc; argumentos++) {
            double media = 0;
        for(int i = 0; i < 20; i++) {
            fil = atoi(argv[argumentos]);
            col = atoi(argv[argumentos]);

            //  Mirar si filas o columnas son potencias de 2
            //  para calcular con cuántos ceros hay que rellenar
            int rellF = 0, rellC = 0;
                //  Comprobamos si col es potencia de dos
                int entero = log2(col);
                if(log2(col) - entero != 0.0){
                    //  Hay que rellenar con 0s algunas columnas
                    //  Calculamos cuantas columnas hay que rellenar
                    int potenciaR = log2(col) + 1;
                    rellC = pow(2, potenciaR) - col;
                    rellF = pow(2, potenciaR) - fil;
                }
                rellF = (col + rellF) - fil;
                rellC = (fil + rellF) - col;

            //  Inicializamos M
            M = new int *[fil+rellF];
            for(int i = 0; i < fil + rellF; i++){
                M[i] = new int[col + rellC];
            }

            //  Rellenamos M
            for(int i = 0; i < fil + rellF; i++){
                for(int j = 0; j < col + rellC; j++){
                    //  SOLO si esta entre las filas y las columnas dadas por el usuario
                    if( i < fil && j < col)
                        M[i][j] = rand()%10;
                    //  Si columnas o filas estan fuera del rango dado
                    else
                            M[i][j] = 0;
                    
                }
            }

            //  Inicializamos N
            N = new int *[fil + rellF];
            for(int i = 0; i < fil + rellF; i++ ){
                N[i] = new int[col + rellC];
            }

            cerr << "Trasposicion de matriz sin divide y venceras tamaño: " << fil << " filas x columnas" << endl;

            //  Obtenemos la hora en la que comienza la ejecucion del algoritmo
            t0 = std::chrono::high_resolution_clock::now();

            //  Ejecución del algoritmo
            for(int i = 0; i < fil+rellF; i++){
                for(int j = 0; j < col + rellC; j++){
                    N[j][i] = M[i][j];
                }
            }

            //  Obtenemos la hora en la que finaliza la ejecución del programa
            tf = std::chrono::high_resolution_clock::now();

            unsigned long tejecucion = std::chrono::duration_cast<std::chrono::microseconds>(tf - t0).count();

            cerr << "\tTiempo de ejec- (us): " << tejecucion << " para tam. caso " << fil << endl;

            media += tejecucion;

            //  Guardamos tam. de caso y tiempo de ejecucion en fichero de salida
            salida << fil << " " << tejecucion << "\n";

            //  Liberamos memoria de las matrices

            for(int i = 0; i < fil+rellF; i++ ){
                    delete [] M[i];
            }
            delete [] M;

            for(int i = 0; i < fil+rellF; i++ ){
                    delete [] N[i];
            }
            delete [] N;

        }

        media = media/20;
        salidamedias << fil << " " << media << "\n";
    }

    //  Cerramos fichero de salida
    salida.close();
    salidamedias.close();

    return 0;

}