//	Sentencia para compilar el código
//  g++ -std=c++11 programaciondinamica.cpp -o programaciondinamica
//	Sentencia para ejecutar el código
// ./programaciondinamica att48.tsp <número de nodos a procesar>
// ./programaciondinamica ulysses16.tsp
// ./programaciondinamica ulysses22.tsp
// ./programaciondinamica a280.tsp <número de nodos a procesar>
#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>

using namespace std;

struct nodo{
	int id;
	double x;
	double y;
};

void calcular_distancias(vector<nodo> &nodos, vector<vector<int> > &distancias){
	double x1, y1, x2, y2;

	for(int i = 0; i<nodos.size(); i++){
		x1 = nodos[i].x;
		y1 = nodos[i].y;

		for(int j = 0; j < nodos.size(); j++){
			if(i == j) 
				distancias[i][j] = 0;
			else{
				x2 = nodos[j].x; 
				y2 = nodos[j].y;
				distancias[i][j] = std::round(sqrt( (x2-x1)*(x2-x1) + (y2-y1)*(y2-y1) ) );
			}		
		}	
	}
}


pair<vector<int>, int> algoritmo_pd(int bitmask, int actual, pair<vector<int>,int>** &subproblema, const vector<vector<int> >& distancias) {

    vector<int> camino;

	//	Si bitmask dice que ya lo hemos visitado
    if(subproblema[bitmask-1][actual].second!=-1) {
        return subproblema[bitmask - 1][actual];
    }

	//	Hemos terminado
    if(bitmask == (1 << distancias.size()) -1) {
        camino.push_back(actual);
        return make_pair(camino, distancias[actual][0]);
    }

    int min = INT_MAX;

	//	Tránsito
    for(int i=0; i<distancias.size(); i++){

        int res = bitmask & (1 << i);

        if(res == 0){

            int nuevoBitmask = bitmask | (1 << i);

            pair<vector<int>, int> nuevoPar = algoritmo_pd(nuevoBitmask, i, subproblema, distancias);

            int nuevoMin = distancias[actual][i] + nuevoPar.second;

            vector<int> nPath(nuevoPar.first);

            nPath.push_back(actual);

			//	Añadimos memoizacion
            if(nuevoMin < min) {
                min = nuevoMin;
                camino = nPath;
            }
        }
    }

	//	Devolvemos la solución
    subproblema[bitmask-1][actual].second = min;

    subproblema[bitmask-1][actual].first = camino;

    return make_pair(camino, min);
}

pair<vector<int>,int>** inicializarSubset(const vector<vector<int> >& distancias){
	pair<vector<int>,int>** subproblema;

	int numCol = pow(2,distancias.size())-1;
    subproblema = new pair<vector<int>, int>*[numCol];
    for(int i=0; i<numCol; i++) {
        subproblema[i] = new pair<vector<int>, int>[distancias.size()];
        for(int j=0;j<distancias.size();j++)
            subproblema[i][j].second = -1;
    }

	return subproblema;
}

pair<vector<int>, int> ejecutarAlgoritmo(const vector<vector<int> >& distancias){
    int bitmask = 1, actual = 0;
    chrono::time_point<chrono::system_clock> t1, t2;
    chrono::duration<double> tiempo;
    pair<vector<int>, int> solucion;
	pair<vector<int>,int>** subproblema = inicializarSubset(distancias);

	t1 = chrono::system_clock::now();
	solucion = algoritmo_pd(bitmask, actual, subproblema, distancias);
	t2 = chrono::system_clock::now();
	tiempo = t2 - t1;
	cout<<"Tiempo (segs) de ejecución del algoritmo dinámico: "<<tiempo.count()<<endl;

	return solucion;
}


int main(int argc, char* argv[]){

	vector<vector<int> > distancias;
	nodo n;
	vector<nodo> nodos;
	ifstream entrada;
	ofstream salida;
	int num_ciudades;
	string basura;
	string comprobante = "NODE_COORD_SECTION";
	bool entramos = true;
	

	if (argc < 2){
		cout << "Uso: " << argv[0] << " ruta_archivo_ciudades <numero de nodos a procesar>" << endl;
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
	if(argv[2]){
		num_ciudades = strtol(argv[2], NULL, 10);
	}
	distancias.resize(num_ciudades);

	//	Reserva de matriz distancias
	for(int i = 0; i < distancias.size(); i++ )
		distancias[i].resize(num_ciudades);
	for (int i=0; entramos; i++){
		getline(entrada, basura);
		if(basura == comprobante){		//	Para delimitar los datos entre NODE_COORD_SECTION
			entramos = false;
		}
	}

	//Metemos en mapa ciudades los datos
	for(int i = 0; i < num_ciudades; i++){

		entrada >> n.id;
		entrada >> n.x;
		entrada >> n.y;

		nodos.push_back(n);
	}

	//Rellenamos la matriz de distancias
	calcular_distancias(nodos, distancias);

	pair<vector<int>, int> solucion = ejecutarAlgoritmo(distancias);

	cout << "Coste del camino: "<<solucion.second << endl;
		//	Mostrar el camino
	for(int i=solucion.first.size()-1; i>=0; i--)
		cout<<solucion.first[i]<<"->";
	cout <<solucion.first[solucion.first.size()-1];
	cout<<endl;
	
	return 0; 
}
