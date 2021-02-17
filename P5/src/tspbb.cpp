#include <iostream>
#include <limits>
#include <cstdlib>
#include <fstream>
#include <vector>
#include <cmath>
#include <queue>
#include <time.h>
#include <string>

using namespace std;

//	Variables globales
double cota_global = numeric_limits<double>::max();
int nodos_explorados = 0, nodos_expandidos = 0;

//	Tipo de dato que almacenará la información de una ciudad
struct Ciudad{
	int id;
	double x;
	double y;
} ciudad;

//	Para el uso de arboles en Branch and Bound
struct Nodo{
	Ciudad ciudad;
	int nivel;
	double c_local;
	vector<bool> visitadas;
	vector<Ciudad> sol_parcial;

	Nodo(Ciudad _ciudad, vector<bool> _visitadas, vector<Ciudad> _sol_parcial, double _cota, int _nivel){
		ciudad = _ciudad;
		visitadas = _visitadas;
		sol_parcial = _sol_parcial;
		c_local = _cota;
		nivel = _nivel;
	}

    bool operator <= (const Nodo &n1)const{
        return this->c_local <= n1.c_local;
    }
    bool operator == (const Nodo &n1)const{
        return this->c_local == n1.c_local;
    }
    bool operator != (const Nodo &n1)const{
        return this->c_local != n1.c_local;
    }
	bool operator > (const Nodo &n1)const{
        return this->c_local > n1.c_local;
    }
    bool operator >= (const Nodo &n1)const{
        return this->c_local >= n1.c_local;
    }
    bool operator < (const Nodo &n1)const{
        return this->c_local < n1.c_local;
    }
    void operator = (const Nodo &n){
    	this->ciudad = n.ciudad;
    	this->visitadas = n.visitadas;
		this->sol_parcial = n.sol_parcial;
		this->c_local = n.c_local;
		this->nivel = n.nivel;
    }

};

//	Esta clase está diseñada para que el priority queue ordene según la cota local de cada nodo
class compararNodo{
	public:
		bool operator()(Nodo n1, Nodo n2){
			return n1.c_local > n2.c_local;
		}
};

//	Método para leer las ciudades desde un fichero
void leerCiudades(vector<Ciudad> &ciudades, const char fichero[], const int n){
	ifstream f(fichero);
	string basura;
	string comprobante = "NODE_COORD_SECTION";
	bool entramos = true;
	double valor_leido = 0.0;
	int num_ciudades = 0;

	if(f){
		for (int i=0; i<3; i++){
			getline(f, basura);
		}

		getline(f, basura, ':'); //	Leo "DIMENSION:"
		f >> num_ciudades; // Número de ciudades
		if(n != -1){
		num_ciudades = n;
		}

		for (int i=0; entramos; i++){
			getline(f, basura);
			if(basura == comprobante){		//	Para delimitar los datos entre NODE_COORD_SECTION
				entramos = false;
			}
		}

		for(int i = 0; i < num_ciudades; i++){
			f >> valor_leido;
			ciudad.id = valor_leido;
			f >> valor_leido;
			ciudad.x = valor_leido;
			f >> valor_leido;
			ciudad.y = valor_leido;

			ciudades.push_back(ciudad);
		}
	}
	f.close();
}

//	Método para calcular la distancia entre dos ciudades
double calcularDistancia(Ciudad a, Ciudad b){
	return sqrt(pow(a.x - b.x, 2) + pow(a.y - b.y, 2));
}

// Funcion para generar una matriz de distancias (FUNCIONA)
vector<vector<double> > matrizDistancias(vector <Ciudad> ciudades){

	vector<vector<double> > salida(ciudades.size());
	double distancia = 0.0;

	for ( int i = 0 ; i < ciudades.size() ; i++ )
	   salida[i].resize(ciudades.size());

	for (int i =0; i < ciudades.size() ; i++){
		for(int j =0; j<ciudades.size();j++){
			if(i==j)
				salida[i][j]=0;
			else{
				distancia = calcularDistancia(ciudades[i], ciudades[j]);			
				salida[i][j]= distancia;
			}		
		}
	}
	return salida;
}

//	Calcula un vector con la distancia mínima desde las ciudades
vector<double> costeMinimo(const vector<vector<double> > &matriz_costes){

	double distancia_minima = numeric_limits<double>::max();
	vector<double> vector_distancias_minimas;

	for(int i = 0; i < matriz_costes.size(); i++){
		for(int j = 0; j < matriz_costes.size(); j++){
			if((matriz_costes[i][j] < distancia_minima) && (i != j))
				distancia_minima = matriz_costes[i][j];
		}
		vector_distancias_minimas.push_back(distancia_minima);
		distancia_minima = numeric_limits<double>::max();
	}

	return vector_distancias_minimas;
}

//	Calcular y devolver la cota local
double getCotaLocal(const vector<Ciudad> &sol_parcial, const vector<double> &vector_distancias_minimas, const vector<vector<double> > &matriz_costes, const vector<bool> visitadas){

	double cota = 0.0;
	bool cerrar_ciclo = true;

	//Añadimos a la cota local la distancia que llevamos hasta el momento
	for(int i = 1; i < sol_parcial.size(); i++)
		cota += matriz_costes[sol_parcial[i-1].id - 1][sol_parcial[i].id - 1];

	//Añadimos a la cota local una estimación optimista para terminar de completar el circuito
	for(int i = 0; i < visitadas.size(); i++){
		if(!visitadas[i]){
			cota += vector_distancias_minimas[i];
			cerrar_ciclo = false;
		}
	}

	//Si ya hemos incluido todas las ciudades, cerramos el ciclo del circuito
	if(cerrar_ciclo)
		cota += matriz_costes[sol_parcial[0].id - 1][sol_parcial[sol_parcial.size()-1].id - 1];
	
	return cota;
}

vector<Nodo> generarNodosVivos(int nivel, vector<bool> visitadas, vector<Ciudad> sol_parcial, 
	const vector<Ciudad> &ciudades, const vector<vector<double> > &matriz_costes, const vector<double> &vector_distancias_minimas){

	vector<Nodo> nodos_vivos;

	for(int i = 0; i < visitadas.size(); i++){
		//	Comprobamos que el nodo vivo que generamos no esté ya generado
		if(visitadas[i] == false){
			nodos_expandidos++;
			//	Incluimos en la información de dicho nodo, la solución parcial que tenía el padre más el propio nodo
			//	formando parte de esta solución
			sol_parcial.push_back(ciudades[i]);
			//	También indicamos en la información del nodo, que esta ciudad ya ha sido visitada
			visitadas[i] = true;
			//	Calculamos la cota local relacionada con este nodo
			double c_local = getCotaLocal(sol_parcial, vector_distancias_minimas, matriz_costes, visitadas);
			Nodo nodo(ciudades[i], visitadas, sol_parcial, c_local, nivel + 1);
			//	Deshacemos todos los cambios para poder seguir generando hijos del mismo padre y que la información no esté alterada
			sol_parcial.pop_back();
			visitadas[i] = false;
			nodos_vivos.push_back(nodo);
		}
	}

	return nodos_vivos;
}

//	Algoritmo Branch & Bound
vector<Ciudad> BranchBoundTSP(const vector<vector<double> > &matriz_costes, const vector<Ciudad> &ciudades, const vector<double> &vector_distancias_minimas){

	priority_queue<Nodo, vector<Nodo>, compararNodo> cola;
	vector<bool> visitadas(ciudades.size(), false);
	vector<Ciudad> sol_parcial, solucion_final;
	vector<Nodo> nodos_vivos;
	sol_parcial.push_back(ciudades[0]);
	int nivel = 0;

	double c_local = getCotaLocal(sol_parcial, vector_distancias_minimas, matriz_costes, visitadas);
	visitadas[0] = true;

	//	Generamos el nodo inicial (ciudad 0) y lo introducimos en la priority queue
	Nodo nodo_inicial(ciudades[0], visitadas, sol_parcial, c_local, nivel);

	cola.push(nodo_inicial);

	//	Mientras que haya nodos vivos
	while(!cola.empty()){
		//	Obtenemos el primer nodo de la cola con prioridad
		Nodo nodo_actual = cola.top();
		cola.pop();
		//	Si es un nodo hoja y su cota local es menor que la global
		if((nodo_actual.nivel == (ciudades.size()-1)) && (nodo_actual.c_local < cota_global)){
			//	Actualizamos la cota global y la solución final
			cota_global = nodo_actual.c_local;
			solucion_final = nodo_actual.sol_parcial;
			nodos_explorados++;
		}
		//	Si el nodo es capaz de alcanzar una mejor solución
		if(nodo_actual.c_local < cota_global){
			nodos_explorados++;
			//	Calculamos los nodos vivos
			nodos_vivos = generarNodosVivos(nodo_actual.nivel, nodo_actual.visitadas, nodo_actual.sol_parcial, ciudades, matriz_costes, vector_distancias_minimas);
			for(int i = 0; i < nodos_vivos.size(); i++)
				cola.push(nodos_vivos[i]);
		}
		//	Si no hay ningún nodo favorable en la lista de nodos vivos, devolvemos la mejor solución, que será, la mejor encontrada hasta entonces
		else{
			return solucion_final;
		}
	}

	return solucion_final;
}

//	Cálculo factorial de un número n
int factorial(int n) { 
   if ((n==0)||(n==1))
      return 1;
   else
      return n*factorial(n-1);
}

// Funcion principal
int main(int argc, char* argv[]){
	clock_t t_ini, t_fin;
	double secs;
	vector<Ciudad> ciudades, solucion_final;
	vector<double> vector_distancias_minimas;
	double distancia = 0.0;
	int nodos_totales;
	
	if (argc < 2){
		cout << "Uso: " << argv[0] << " ruta_archivo_ciudades <numero_de_ciudades>" << endl;
		return (-1);
	}

	//	Leemos los datos
	int n;

	if(argv[2])
		n = strtol(argv[2], NULL, 10);
	else
		n = -1;
	
	leerCiudades(ciudades, argv[1], n);
	//	---------------------
	//	Generamos la matriz de distancias
	vector<vector<double> > matriz = matrizDistancias(ciudades);
	vector_distancias_minimas = costeMinimo(matriz);
	//	---------------------
	//	Calculamos el tiempo de ejecución del algoritmo Branch & Bound
	t_ini = clock();
	solucion_final = BranchBoundTSP(matriz, ciudades, vector_distancias_minimas);
	nodos_totales = factorial(solucion_final.size());
	t_fin = clock();

	secs = (double)(t_fin - t_ini) / CLOCKS_PER_SEC;
	//	---------------------
	//	Sumamos las distancias del recorrido
	for(int i = 1; i < solucion_final.size(); i++){
		distancia += matriz[solucion_final[i-1].id - 1][solucion_final[i].id - 1];
	}
	//	---------------------
	//	Añadimos la distancia de la última ciudad a la primera
	distancia += matriz[solucion_final[0].id - 1][solucion_final[solucion_final.size() - 1].id - 1];
	//	---------------------

	//	Mostramos los resultados
	cout << "TIEMPO (seg): " << secs << endl;
	cout << "DISTANCIA: " << distancia << endl;
	cout << "TOUR: ";
	for(int i = 0; i < solucion_final.size(); i++){
		cout << solucion_final[i].id << " ";
	}
	cout << solucion_final[0].id << endl;

	cout << "NODOS TOTALES: " << nodos_totales << endl;
	cout << "NODOS EXPLORADOS: " << nodos_explorados << endl;
	cout << "NODOS EXPANDIDOS: " << nodos_expandidos << endl;
	cout << "NODOS PODADOS: " << nodos_totales - nodos_explorados << endl;
	// ---------------------

	return 0;
}
