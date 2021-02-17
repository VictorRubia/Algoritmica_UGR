//	Sentencia para compilar el código
//  g++ -std=c++11 algoritmos_comunes.cpp -o algoritmos_comunes
//	Sentencia para ejecutar el código
// ./algoritmos_comunes a280.tsp
// ./algoritmos_comunes att48.tsp
// ./algoritmos_comunes ulysses16.tsp
// ./algoritmos_comunes ulysses22.tsp
#include <iostream>
#include <fstream>
#include <cmath>
#include <fstream>
#include <stdlib.h>
#include <map>
#include <set>
#include <vector>
#include <limits>
#include <cassert>
#include <queue>
#include <chrono> 

using namespace std;

void calcular_distancias(map<int, pair<double, double> > &ciudades, double **distancias, int n ){
	double x1, y1, x2, y2;

	for(int i = 1; i<=n; i++){
		x1 = ciudades[i].first;
		y1 = ciudades[i].second;

		for(int j = 1; j <= n; j++){
			if(i == j) distancias[i-1][j-1] = 0;
			else{
				x2 = ciudades[j].first;
				y2 = ciudades[j].second;

				distancias[i-1][j-1] = std::round(sqrt( (x2-x1)*(x2-x1) + (y2-y1)*(y2-y1) ) );
			}		
		}	
	}
}

int calcularCostePlan(vector<int> & plan, double **distancias){
	int Cactual = 1;
	int coste = 0;
	for(int i=0; i< plan.size(); i++){
		coste += distancias[Cactual-1][plan[i]-1];
		Cactual = plan[i];
	}
	
	coste += distancias[plan[plan.size()-1]-1][plan[0]-1];
	return coste;
}



vector<int> algoritmo_insercion(double **distancias, map<int, pair<double, double> > &ciudades, int n){
	//Declaramos un vector solucion
	vector<int> resul;
	int basura;
	
	//Calcular las ciudades mas alejadas uwu
	int ciudadN, ciudadE, ciudadO;
	int coorN=0, coorE=0, coorO=std::numeric_limits<int>::max();
	for(int i = 1; i<=n; i++){
		if(ciudades[i].first > coorE){
			coorE = ciudades[i].first;
			ciudadE = i-1;
		}
		else if(ciudades[i].first < coorO){//Estructura else if para que no se repitan los nodos que se escogen
			coorO = ciudades[i].first;
			ciudadO = i-1;
		}
		else if(ciudades[i].second > coorN){
			coorE = ciudades[i].second;
			ciudadN = i-1;
		}
	}
	
	//Declaramos la matriz de uniones [0, 1]
	int **uniones;
	uniones = (int**) malloc(n*sizeof(int*));
	for(size_t i=0; i<n; i++){
		uniones[i] = (int*) malloc(n*sizeof(int));
	}

	for(int i=0; i<n; i++){
		for(int j=0; j<n; j++){
			uniones[i][j] = 0;
		}
	}
	//Ponemos como unidas las ciudades
	uniones[ciudadN][ciudadE] = 1;
	uniones[ciudadN][ciudadO] = 1;
	uniones[ciudadE][ciudadO] = 1;

	uniones[ciudadE][ciudadN] = 1;
	uniones[ciudadO][ciudadN] = 1;
	uniones[ciudadO][ciudadE] = 1;
	
	//Rellenamos la cola de los cerrados y los abiertos
	vector<int> cerrados;	
	queue<int> abiertos;
	for(int i=0; i<n; i++){
		if(i != ciudadN && i != ciudadE && i != ciudadO){
			abiertos.push(i);
		}
		else{
			cerrados.push_back(i);
		}
	}
	

	//Algoritmo con cada uno de los nodos pendientes

	while(abiertos.size() != 0){
		int ciudadA = abiertos.front();
		abiertos.pop();
		int sol;
		int distAux = std::numeric_limits<int>::max();
		int nodoAux;

		for(int i=0; i<cerrados.size(); i++){


			sol = ((distancias[cerrados[i]][ciudadA] + distancias[cerrados[(i+1)%cerrados.size()]][ciudadA]) - distancias[cerrados[i]][cerrados[(i+1)%cerrados.size()]]) ;

			if(sol < distAux){
				distAux = sol;
				nodoAux = i;
			}
		}

		uniones[cerrados[nodoAux]][cerrados[(nodoAux+1)%cerrados.size()]] = 0;
		uniones[cerrados[(nodoAux+1)%cerrados.size()]][cerrados[nodoAux]] = 0;
		uniones[ciudadA][cerrados[nodoAux]] = 1;
		uniones[cerrados[nodoAux]][ciudadA] = 1;	
		uniones[ciudadA][cerrados[(nodoAux+1)%cerrados.size()]] = 1;
		uniones[cerrados[(nodoAux+1)%cerrados.size()]][ciudadA] = 1;
		
		//INSERTAR EL NUEVO NODO DETRAS DE nodoAUX
		//Buscamos el iterador de nodoAux
		int i = 0;
		auto it = cerrados.begin();

		while(i != nodoAux+1){
			++it;
			i++;
		}

		cerrados.insert(it, ciudadA);
				
	}
	//Asignamos a resul los valores de los cerrados transformados en ciudades
	for(int i =0; i < n; i++)	
		resul.push_back(cerrados[i]+1);
	
	return resul;
}


vector<int> algoritmo_cercania(double **distancias, int n){
	int ciudad_actual = 0;
	int basura;
		//Marcamos como no validos las distancias a la ciudad 0 (inicio)
		for(int i = 0; i<n; i++){
			distancias[i][ciudad_actual] = 0;
		}

	vector<int> resul;
	resul.push_back(ciudad_actual+1);
	//Recorremos todas las ciudades
	for(int i=0; i<n-1; i++){
		//Buscamos la ciudad con distancia min != 0
		int sig ;
		int distancia = std::numeric_limits<int>::max();
		for(int j=0; j<n; j++){
			if (distancias[ciudad_actual][j] < distancia && distancias[ciudad_actual][j] != 0){
				distancia = distancias[ciudad_actual][j];
				sig = j;
			}
		}
		resul.push_back(sig+1);
		//Marcamos como no validos las distancias de la ciudad que ya hemos cogido
		for(int i = 0; i<n; i++){
			distancias[i][sig] = 0;
		}
		ciudad_actual = sig;
	}
	

	return resul;
}

vector<int> algoritmo_intercambio(double **distancias, map<int, pair<double, double> > &ciudades, int n){
	int aux;
	int pos;
	vector<int> resul, S_aux;
	int coste_aux;
	bool mejor_enc, cambio = true;

	//La solucion inicial es la de insercion
	resul = algoritmo_insercion(distancias, ciudades, n);
	double mejor_coste = calcularCostePlan(resul, distancias);

	cout << "Coste inicial: " << mejor_coste << endl;

	while(cambio){
		cambio = false;
		for(int i = 0; i<resul.size()-1; i++){
				mejor_enc=false;
			for(int j = i+1; j< resul.size(); j++){
				S_aux = resul;
				aux= S_aux[i];
				S_aux[i]=S_aux[j];
				S_aux[j]=aux;
				
				coste_aux = calcularCostePlan(S_aux, distancias);
				if(coste_aux < mejor_coste){
					mejor_enc = true;
					pos = j;
					mejor_coste = coste_aux;
				}
			}
			if(mejor_enc){
				cambio = true;
				aux = resul[i];
				resul[i] = resul[pos];
				resul[pos] = aux;
			}
		}
	}
	
	cout << "Coste final: " << mejor_coste << endl;
	
	return resul;
	
}

int main(int argc, char* argv[]){
	double **distancias;
	map<int, pair<double, double> > ciudades;
	ifstream entrada;
	ofstream salida;
	int num_ciudades;
	string basura;
	string comprobante = "NODE_COORD_SECTION";
	bool entramos = true;
	std::chrono::high_resolution_clock::time_point t1, tTotal;
	

	if (argc != 2){
		cout << "Uso: " << argv[0] << " ruta_archivo_ciudades" << endl;
		return (-1);
	}

	entrada.open(argv[1]);


	if (!entrada.is_open()){
		cout << "Fallo al abrir el archivo de entrada" << endl;
		return (-2);
	}

	//Lecura de los datos de las ciudades
	for (int i=0; i<3; i++){
	    getline(entrada, basura);
	}	

	getline(entrada, basura, ':'); //	Leo "DIMENSION:"
	entrada >> num_ciudades; // Número de ciudades
	cout << num_ciudades << endl;

	for (int i=0; entramos; i++){
		getline(entrada, basura);
		if(basura == comprobante){		//	Para delimitar los datos entre NODE_COORD_SECTION
			entramos = false;
		}
	}

	//Inicializamos la matriz de distancias
	distancias = (double**) malloc(num_ciudades*sizeof(double*));
	for(size_t i=0; i<num_ciudades; i++){
		distancias[i] = (double*) malloc(num_ciudades*sizeof(double));
	}
				

	//Metemos en mapa ciudades los datos
		for(int i = 0; i < num_ciudades; i++){
			double numC, x, y;
			pair<double,double> aux;
			entrada >> numC;
			entrada >> x;
			entrada >> y;
			//cout << "Ciudad: " << numC << " " << x << " " << y << endl;
			aux.first = x; aux.second = y;
			ciudades[numC] = aux;
		}
	
	//Rellenamos la matriz de distancias
	calcular_distancias(ciudades, distancias, num_ciudades);
	
	unsigned tc0, tc1, ti0, ti1, tchan0, tchan1;
 
	
 


/***********************************************************************************************************/	
	cout << "ALGORITMO DE CERCANÍA: " << endl;
	tc0=clock();
	vector<int> cercania = algoritmo_cercania(distancias, num_ciudades);
	tc1=clock();
	for(int i =0; i < cercania.size(); i++){
		cout << cercania[i] << " -> " ;
	}
	cout << cercania[0];
	cout << endl;

	calcular_distancias(ciudades, distancias, num_ciudades);
	cout << "El coste del camino es: " << calcularCostePlan(cercania, distancias) << endl << endl << endl;
/***********************************************************************************************************/	
	
	cout << "ALGORITMO DE INSERCION: " << endl;
	ti0=clock();
	vector<int> insercion = algoritmo_insercion(distancias, ciudades, num_ciudades);
	ti1=clock();
	for(int i =0; i < insercion.size(); i++)
		cout << insercion[i] << " -> " ;
	cout << insercion[0];
	cout << endl;
	
	calcular_distancias(ciudades, distancias, num_ciudades);
	cout << "El coste del camino es: " << calcularCostePlan(insercion, distancias) << endl << endl << endl;
/***********************************************************************************************************/	
	
	cout << "ALGORITMO DE INTERCAMBIO: " << endl;
	tchan0=clock();
	vector<int> intercambio = algoritmo_intercambio(distancias, ciudades, num_ciudades);
	tchan1=clock();
	for(int i =0; i < intercambio.size(); i++)
		cout << intercambio[i] << " -> ";
	cout << intercambio[0];
	cout << endl;
	
	calcular_distancias(ciudades, distancias, num_ciudades);
	cout << "El coste del camino es: " << calcularCostePlan(intercambio, distancias) << endl << endl << endl;
/***********************************************************************************************************/	
	
	double timeCercania = (double(tc1-tc0)/CLOCKS_PER_SEC);
	double timeInsercion = (double(ti1-ti0)/CLOCKS_PER_SEC);
	double timeIntercambio = (double(tchan1-tchan0)/CLOCKS_PER_SEC);
	cout << "Los tiempos de ejecución son CERCANIA: " << timeCercania << " INSERCION " << timeInsercion << " y INTERCAMBIO "  << timeIntercambio << endl;
	
}
