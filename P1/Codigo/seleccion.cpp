/**
   @file Ordenación por selección
*/

   
#include <iostream>
using namespace std;
#include <ctime>
#include <cstdlib>
#include <climits>
#include <cassert>
// Ejemplo de la ordenacion por burbuja sobre un vector de enteros

#include <cstdlib> // Para usar srand y rand
#include <chrono>
#include <iostream>
#include <fstream> // Para usar ficheros






/* ************************************************************ */ 
/*  Método de ordenación por selección  */

/**
   @brief Ordena un vector por el método de selección.

   @param T: vector de elementos. Debe tener num_elem elementos.
             Es MODIFICADO.
   @param num_elem: número de elementos. num_elem > 0.

   Cambia el orden de los elementos de T de forma que los dispone
   en sentido creciente de menor a mayor.
   Aplica el algoritmo de selección.
*/
inline static 
void seleccion(int T[], int num_elem);



/**
   @brief Ordena parte de un vector por el método de selección.

   @param T: vector de elementos. Tiene un número de elementos 
                   mayor o igual a final. Es MODIFICADO.
   @param inicial: Posición que marca el incio de la parte del
                   vector a ordenar.
   @param final: Posición detrás de la última de la parte del
                   vector a ordenar. 
		   inicial < final.

   Cambia el orden de los elementos de T entre las posiciones
   inicial y final - 1de forma que los dispone en sentido creciente
   de menor a mayor.
   Aplica el algoritmo de selección.
*/
static void seleccion_lims(int T[], int inicial, int final);



/**
   Implementación de las funciones
**/

void seleccion(int T[], int num_elem)
{
  seleccion_lims(T, 0, num_elem);
}

static void seleccion_lims(int T[], int inicial, int final)
{
  int i, j, indice_menor;
  int menor, aux;
  for (i = inicial; i < final - 1; i++) {
    indice_menor = i;
    menor = T[i];
    for (j = i; j < final; j++)
      if (T[j] < menor) {
	indice_menor = j;
	menor = T[j];
      }
    aux = T[i];
    T[i] = T[indice_menor];
    T[indice_menor] = aux;
  };
}



int main(int argc, char *argv[]) {
  
  int *v;
  int n, i, argumento;
    chrono::time_point<std::chrono::high_resolution_clock> t0, tf; // Para medir el tiempo de ejecución
  double tejecucion; // tiempo de ejecucion del algoritmo en ms
  unsigned long int semilla;
  ofstream fsalida;
  
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
  
  // Inicializamos generador de no. aleatorios
  semilla= atoi(argv[2]);
  srand(semilla);
  
  // Pasamos por cada tamaÒo de caso
  for (argumento= 3; argumento<argc; argumento++) {
    
    // Cogemos el tamanio del caso
    n= atoi(argv[argumento]);
    
    // Reservamos memoria para el vector
    v= new int[n];
    
    // Generamos vector aleatorio de prueba, con componentes entre 0 y n-1
    for (i= 0; i<n; i++)
      v[i]= rand()%n;
    
    cerr << "Ejecutando Burbuja para tam. caso: " << n << endl;
    
    t0= std::chrono::high_resolution_clock::now(); // Cogemos el tiempo en que comienza la ejecuciÛn del algoritmo
    seleccion(v, n); // Ejecutamos el algoritmo para tamaÒo de caso tam
    tf= std::chrono::high_resolution_clock::now(); // Cogemos el tiempo en que finaliza la ejecuciÛn del algoritmo
    
    unsigned long tejecucion= std::chrono::duration_cast<std::chrono::microseconds>(tf - t0).count();
    
    cerr << "\tTiempo de ejec. (us): " << tejecucion << " para tam. caso "<< n<<endl;
    
    // Guardamos tam. de caso y t_ejecucion a fichero de salida
    fsalida<<n<<" "<<tejecucion<<"\n";
    
    
    // Liberamos memoria del vector
    delete [] v;
  }
  
  // Cerramos fichero de salida
  fsalida.close();
  
  return 0;
}
