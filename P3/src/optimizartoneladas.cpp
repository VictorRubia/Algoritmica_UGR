//	Sentencia para compilar el código
//  g++ -std=c++11 optimizartoneladas.cpp -o optimizartoneladas
#include <iostream>
#include <vector>
#include <cstdlib>
#include <chrono>
using namespace std;

//  Obtención del contenedor con mayor peso
int seleccion(const vector <int> &contenedor) {
    int max=contenedor[0];
    int maxpos=0;
    int p;
    for(int i=1; i<contenedor.size(); i++) {
        p=contenedor[i];
        if(max <=p) {
            max=p;
            maxpos=i;
        }
    }
    return maxpos;
}

//  Entrada: vector de contenedores y peso máximo del barco
//  Retorna un vector con los contenedores cargados
vector <int> Barco(vector <int> contenedor ,int tam_max) {
    vector<int> solucion;
    int obj;
    int candidato;//posicion del vector de candidatos
    int peso_actual=0;
    while(!contenedor.empty() && peso_actual<=tam_max) {
        candidato= seleccion(contenedor);
        obj =contenedor[candidato];
        contenedor[candidato]=contenedor[contenedor.size()-1];
        contenedor.pop_back();
        if((peso_actual + obj) <= tam_max) { //funcion de factibilidad
            solucion.push_back(obj);
            peso_actual+=obj;
        }
    }
    return solucion;
}


//  Genera un vector de tamaños aleatorios
vector<int> genera(int n) {
    vector<int> T(n);
    srand(time(0));
    for(int i=0; i<n; i++) {
        T[i]=(rand()%100)+1;//aleatorio entre 1-100
    }
    return T;
}

int main(int argc, char *argv[]) {
    if(argc != 2) {
        cerr << "Formato " << argv[0] << " <numero contenedores>" << "\n";
        return -1;
    }
    //variables para el tiempo
    std::chrono::high_resolution_clock::time_point tantes, tdespues;
    std::chrono::duration<double> transcurrido;
    int n = atoi(argv[1]);
    int tamamax=40*n;//tamaño de los contenedores en funcion del nuemero de contenedores
    vector <int> prueba;
    prueba=genera(n);
    vector <int> s;
    tantes = std::chrono::high_resolution_clock::now(); //coger tiempo antes de la operación
    s=Barco(prueba,tamamax);
    tdespues = std::chrono::high_resolution_clock::now();//coger tiempo después de la operación
    transcurrido = std::chrono::duration_cast<std::chrono::duration<double>>(tdespues - tantes);
    std::cout <<"Para tamaño: "<<n<< " tiempo: " << transcurrido.count() << "\n";
    /*for(int i=0;i<s.size();i++)
    	cout<<s[i]<<" ";
    cout<<endl;*/
}
