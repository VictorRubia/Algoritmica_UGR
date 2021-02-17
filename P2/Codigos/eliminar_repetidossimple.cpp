//  Compilación --> g++ -std=c++11 eliminar_repetidossimple.cpp -o eliminar_repetidossimple
#include <iostream>

#include <stdio.h>      /* printf, scanf, puts, NULL */
#include <stdlib.h>     /* srand, rand */
#include <assert.h> 
#include <vector>
#include <fstream>
#include <ctime>
#include <cstdlib>
#include <climits>
#include <cassert>
#include <chrono>

using namespace std;

void eliminarRepSimple( vector<int> &v ){

	for(int i = 0; i < v.size(); i++)
		for(int j=i+1; j < v.size(); j++)
			if(v[i] == v[j]){
				v.erase(v.begin() + j);
                j--;
            }

}

//generador de ejemplos para el problema de eliminar elementos repetidos. Simplemente, para rellenar el vector de tamaño n genera n enteros aleatorios entre 0 y n-1
double uniforme(){
	int t = rand();
	double f = ((double)RAND_MAX+1.0);
	return (double)t/f;
}

int main(int argc, char* argv[]){

    int argumentos, tam_vec;
    chrono::time_point<std::chrono::high_resolution_clock> t0, tf;
    ofstream salida;
	ofstream salidamedias;
    unsigned long int sem;

	if (argc <= 3) {
		cerr<<"\nError: El programa se debe ejecutar de la siguiente forma.\n\n";
		cerr<<argv[0]<<" NombreFicheroSalida Semilla tamCaso1 tamCaso2 ... tamCasoN\n\n";
		return 0;
	}

    //  Abrimos fichero de salida
    salida.open(argv[1]);
    if (!salida.is_open()){
        cerr << "Error: No se pudo abrir fichero para escritura " << argv[1] << "\n\n";
        return 0;
    }

    //  Abrimos fichero de cálculo de medias
    salidamedias.open("Media_eliminar_repetidos_simple.txt");
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
        for(int i = 0; i < 10; i++) {
            
            int n = atoi(argv[argumentos]);

            vector<int> v(n);
            tam_vec = v.size();
            //para generar un vector que pueda tener elementos repetidos
            for (int j=0; j<n; j++) {
                double u=uniforme();
                int k=(int)(n*u);
                v[j]=k;
            }


            //  Obtenemos la hora en la que comienza la ejecucion del algoritmo
            t0 = std::chrono::high_resolution_clock::now();

		    eliminarRepSimple(v);

            //  Obtenemos la hora en la que finaliza la ejecución del programa
            tf = std::chrono::high_resolution_clock::now();

            unsigned long tejecucion = std::chrono::duration_cast<std::chrono::microseconds>(tf - t0).count();

            cerr << "\tTiempo de ejec- (us): " << tejecucion << " para tam. caso " << tam_vec << endl;

            media += tejecucion;

            //  Guardamos tam. de caso y tiempo de ejecucion en fichero de salida
            salida << tam_vec << " " << tejecucion << "\n";

            //  Liberamos memoria de las matrices

            v.clear();

        }

        media = media/10;
        salidamedias << tam_vec << " " << media << "\n";
    }
	
    salida.close();
    salidamedias.close();

	return 0;
}