#include <stdlib.h> // rand
#include <time.h> // Time()

#include <vector>
#include <iostream>
#include <ctime>
#include <cstdlib>
#include <climits>
#include <cassert>
#include <string>
#include <fstream>
#include <chrono>
using namespace std::chrono;
high_resolution_clock::time_point tantes, tdespues;
duration<double> transcurrido;


using namespace std;

int cota_global_max = 0;

// Función que genera una entrada para nuestro problema
// la posicion i,j es igual a la j,i y representa la compatibilidad
// entre los asistentes i y j.
int **generarEntrada(int n){

	srand(time(NULL)); // inicializa srand
	int **matriz;
	matriz = new int*[n];

	for (int i = 0; i < n; i++)
		matriz[i] = new int[n];

	for (int i = 0; i < n ;i++){
		for (int j = i; j < n; j++){
			if (i==j)
				matriz[i][j] = 0;
			else{
				int num = 1 + ( rand() % (100) );
				matriz[i][j] = num;
				matriz[j][i] = num;
			}
		}
	}

	return matriz;
}


//	Vamos recorriendo todos los comensales que están sentados, y en función de eso calculamos la suma del
//	que está sentado a su izquierda y a su derecha
int calcularValorActual(int n_comensales, int solucion[], vector<bool> &comensales_sentados, int **matriz){

	vector<int> indices_comensales_sentados; //	Vamos a guardar los índices de los comensales que están sentados
	int valor = 0;

	for(int i = 0; i < n_comensales; i++){
		if(comensales_sentados[i] == true){ //	Si el comensal está sentado
			int indice = 0;
			while(indice < n_comensales){
				if(solucion[indice] == i) //	Buscamos dentro de nuestra solución donde se encuentra dicho comensal
					indices_comensales_sentados.push_back(indice);
				indice++;
			}

		}
	}

	for(int i = 1; i < indices_comensales_sentados.size(); i++){
		valor += matriz[indices_comensales_sentados[i-1]][indices_comensales_sentados[i]]; //	Sumamos siempre con el que está a nuestra izquierda
	}

	if(indices_comensales_sentados.size() > 2)
		valor += matriz[0][indices_comensales_sentados.back()]; //	Si hay más de un comensal sentado, sumamos los extremos

	return (valor * 2); //	Multiplicamos por dos, ya que así obtenemos el resultado de sumar con el de tu izquierda y derecha

}


void backtracking (int n_comensales, int nivel, int comensal, int &cota_local, int solucion[], vector<bool> &comensales_sentados, int **matriz, int solucion_final[]) {

	comensales_sentados[comensal] = true; //	Sentamos al comensal para el que estamos calculando la posible solución
	solucion[nivel - 1] = comensal;
	cota_local = 0;

	for (int siguiente_comensal = 0; siguiente_comensal < n_comensales; siguiente_comensal++) { //	Recorremos todos los comensales
		//	Si el comensal que estamos mirando no está sentado pasamos a evaluar
		if (comensales_sentados[siguiente_comensal] == false) {
			//	Calculamos la cota que tenemos hasta el momento
			cota_local = calcularValorActual(n_comensales, solucion, comensales_sentados, matriz);
			//	Descendemos en profundidad para evaluar sus hijos.
			backtracking(n_comensales, nivel + 1, siguiente_comensal, cota_local, solucion, comensales_sentados, matriz, solucion_final);

			if (nivel == (n_comensales - 1)) { //	Si estamos en un nodo hoja
				//	Calculamos la cota total que obtenemos con todos los comensales sentados
				cota_local = calcularValorActual(n_comensales, solucion, comensales_sentados, matriz);
				//	Si la cota que obtenemos es mejor que la máxima que hemos obtenido hasta el momento la actualizamos y guardamos la solución
				if (cota_local > cota_global_max) {
					for(int i = 0; i < n_comensales; i++)
						solucion_final[i] = solucion[i];
					cota_global_max = cota_local;
				}
			}else{ //	Si no estamos en un nodo hoja
				cota_local = calcularValorActual(n_comensales, solucion, comensales_sentados, matriz);
			}
			//	Levantamos al comensal
			comensales_sentados[siguiente_comensal] = false;
		}

	}
}

int main(int argc, char const *argv[]){

 	if(argc < 2){
		cout << "Número de argumentos inválido. Pruebe con ./programa <número_comensales>" << endl;
		exit(1);
	} 

	int n = atoi(argv[1]);
	int cota_local = 0;
	vector<bool> comensales_sentados(n, false);
	int solucion[n], solucion_final[n];
	int **matriz = generarEntrada(n);

	tantes = high_resolution_clock::now();
	backtracking(n, 1, 0, cota_local, solucion, comensales_sentados, matriz, solucion_final);
	tdespues = high_resolution_clock::now();
    
    transcurrido = duration_cast<duration<double> >(tdespues - tantes);
	cout << "Para " << n << " comensales" << " se tarda en ordenar "<< transcurrido.count() << endl;
    cout << "Se sientan así:" << endl;
    for(int i = 0; i < n; i++){
        cout << solucion_final[i] << " ";
    }
    cout << "\n(Donde el primero se sienta al lado del último)" << endl;
	return 0;
}
