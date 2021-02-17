/**
   @file Resolución del problema de las Torres de Hanoi
*/

   
#include <iostream>
using namespace std;
#include <ctime>
#include <cstdlib> // Para usar srand y rand
#include <chrono>
#include <iostream>
#include <fstream> // Para usar ficheros
#include <climits>
#include <cassert>


/**
   @brief Resuelve el problema de las Torres de Hanoi
   @param M: número de discos. M > 1.
   @param i: número de columna en que están los discos.
             i es un valor de {1, 2, 3}. i != j.
   @param j: número de columna a que se llevan los discos.
             j es un valor de {1, 2, 3}. j != i.

   Esta función imprime en la salida estándar la secuencia de 
   movimientos necesarios para desplazar los M discos de la
   columna i a la j, observando la restricción de que ningún
   disco se puede situar sobre otro de tamaño menor. Utiliza
   una única columna auxiliar.
*/
void hanoi (int M, int i, int j);




void hanoi (int M, int i = 1, int j = 2)
{
  if (M > 0)
    {
      hanoi(M-1, i, 6-i-j);
      //cout << i << " -> " << j << endl;
      hanoi (M-1, 6-i-j, j);
  }
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
    hanoi(n, 1 , 2); // Ejecutamos el algoritmo para tamaÒo de caso tam
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