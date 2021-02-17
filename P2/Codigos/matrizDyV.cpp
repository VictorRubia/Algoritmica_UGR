#include <iostream>
#include <cstdlib>
#include <math.h>
#include <chrono>
#include <fstream>
using namespace std;





void intercambiar(int **m, int iniciofa, int inicioca, int iniciofb, int iniciocb, int dimension){
	for(int i=0; i<=dimension-1; i++){
		for(int j=0; j<=dimension-1; j++){
			int aux = m[iniciofa+i][inicioca+j];
			m[iniciofa+i][inicioca+j] = m[iniciofb+i][iniciocb+j];		
			m[iniciofb+i][iniciocb+j] = aux;
			
		}
	}
}

void trasponerM(int **m, int iniciof, int finalf, int inicioc, int finalc){
	if(iniciof<finalf){
		int mitadf = ((iniciof+finalf)/2);
		int mitadc = ((inicioc+finalc)/2);
		trasponerM(m, iniciof, mitadf, inicioc, mitadc);
		trasponerM(m, iniciof, mitadf, mitadc+1, finalc);
		trasponerM(m, mitadf+1, finalf, inicioc, mitadc);
		trasponerM(m, mitadf+1, finalf, mitadc+1, finalc);
		intercambiar(m, mitadf+1, inicioc, iniciof, mitadc+1, finalf-mitadf);
		
		
	}
}


void trasponer(int ** m, int filas, int columnas){
	trasponerM(m, 0, filas-1, 0, columnas-1);
}






int main(int argc, char *argv[]){

	int **M;
	int n, i, argumento;
	int filas, columnas;
    chrono::time_point<std::chrono::high_resolution_clock> t0, tf; // Para medir el tiempo de ejecución
	double tejecucion; // tiempo de ejecucion del algoritmo en ms
	unsigned long int semilla;
	ofstream fsalida;
	ofstream fsalidaMEDIA;
	
	if (argc <= 3) {
		cerr<<"\nError: El programa se debe ejecutar de la siguiente forma.\n\n";
		cerr<<argv[0]<<" NombreFicheroSalida Semilla tamCaso1 tamCaso2 ... tamCasoN\n\n";
		return 0;
	}
	
	// Abrimos fichero de salida
	fsalida.open(argv[1]);
	if (!fsalida.is_open()) {
		cerr<<"Error: No se pudo abrir fichero para escritura "<<argv[1]<<"\n\n";
		return 0;
	}

	fsalidaMEDIA.open("MEDIASMATRICESDYV");
	if (!fsalidaMEDIA.is_open()) {
		cerr<<"Error: No se pudo abrir fichero para escritura MEDIASMATRICESDYV "<<"\n\n";
		return 0;
	}
	
	// Inicializamos generador de no. aleatorios
	semilla= atoi(argv[2]);
	srand(semilla);

//Pasamos por cada tamaño de caso
	for (argumento= 3; argumento<argc; argumento++) {
		double media = 0;
		for(int j=0; j<20; j++){

			filas= atoi(argv[argumento]);
			columnas= atoi(argv[argumento]);


			//Mirar si filas o columnas son potencias de 2
			//Caclular cuantos ceros hay que rellenar
			int rellenoF = 0, rellenoC = 0;
				//mirar si filas y columnas es potencia de dos (Lo hacemos con cualquiera ya que FILAS==COLUMNAS)

				int entero = log2(columnas);
				if(log2(columnas) - entero  != 0.0){
					//Hay que rellenar con 0 algunas columnas
					//Calculamos cuantas columnas hay que rellenar
					int potenciaR = log2(columnas) + 1;
					rellenoC = pow(2, potenciaR) - columnas;
					rellenoF = pow(2, potenciaR) - filas;
				}
				rellenoF = (columnas + rellenoC) - filas;
				rellenoC = (filas + rellenoF) - columnas;
			M = new int *[filas+rellenoF];
			for(int i = 0; i < filas+rellenoF; i++ ){
					M[i] = new int[columnas+rellenoC];
			}
			//Rellenamos la matriz
			for(int i=0; i<filas+rellenoF; i++){
				for(int j=0; j<columnas+rellenoC; j++){
					//SOLO si esta entre las filas y las columnas dadas por el usuario
					if(i<filas && j<columnas)
						M[i][j] = rand()%10;
					//Si columnas o filas estan fuera del rango dado		
					else
						M[i][j] = 0;
				}
			}

			cerr << "Trasposicion de matriz DIVIDE Y VENCERAS tamaño: " << filas << " filas x columnas" << endl;



			t0= std::chrono::high_resolution_clock::now(); // Cogemos el tiempo en que comienza la ejecuciÛn del algoritmo
			trasponer(M, filas+rellenoF, columnas+rellenoC);
			tf= std::chrono::high_resolution_clock::now(); // Cogemos el tiempo en que finaliza la ejecuciÛn del algoritmo

			unsigned long tejecucion= std::chrono::duration_cast<std::chrono::microseconds>(tf - t0).count();
				
			cerr << "\tTiempo de ejec. (us): " << tejecucion << " para tam. caso "<< filas <<endl;

			media += tejecucion;
			
			// Guardamos tam. de caso y t_ejecucion a fichero de salida
			fsalida<< filas <<" "<<tejecucion<<"\n";
			
			
			// Liberamos memoria de las matrices
			
			for(int i = 0; i < filas+rellenoF; i++ ){
					delete [] M[i];
			}
			delete [] M;

			

		}
		
		media = media/20;
		fsalidaMEDIA << filas <<" "<< media << "\n";
	}


	// Cerramos fichero de salida
	fsalida.close();
	fsalidaMEDIA.close();
	
	return 0;

}
