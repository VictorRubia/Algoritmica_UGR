// Compilación --> g++ -std=c++11 eliminar_repetidosDyV.cpp -o eliminar_repetidosDyV
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

void Merge(vector<int> &v,int p, int q,int r)     
{

    int n1,n2,i,j,k; 
    //size of left array=n1
    //size of right array=n2       
    n1=q-p+1;
    n2=r-q;             
    int L[n1],R[n2];
    //initializing the value of Left part to L[]
    for(i=0;i<n1;i++)
    {
        L[i]=v[p+i];
    }
    //initializing the value of Right Part to R[]
    for(j=0;j<n2;j++)
    {
        R[j]=v[q+j+1];
    }
    i=0,j=0;
    //Comparing and merging them
    //into new array in sorted order 
    for(k=p;i<n1&&j<n2;k++)
    {
        if(L[i]<R[j])
        {
            v[k]=L[i++];
        }
        else
        {
            v[k]=R[j++];
        }
    }
    //If Left Array L[] has more elements than Right Array R[]
    //then it will put all the
    // reamining elements of L[] into A[]
    while(i<n1)             
    {
        v[k++]=L[i++];
    }
    //If Right Array R[] has more elements than Left Array L[]
    //then it will put all the
    // reamining elements of L[] into A[]
    while(j<n2)
    {
        v[k++]=R[j++];
    }
}

void MergeSort(vector<int> &v,int p,int r)    
{
    int q;                                
    if(p<r)
    {
        q=(p+r)/2;
        MergeSort(v,p,q);
        MergeSort(v,q+1,r);
        Merge(v,p,q,r);
    }
}

void eliminarRepDV(vector<int> &v) {

    MergeSort(v, 0, v.size()-1);

    for (int i = 0; i < v.size() - 1; ++i) {
        if (v[i] == v[i+1]) {
            v.erase(v.begin()+i+1);
            i--;
        }
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
    salidamedias.open("Media_elminar_repetidosDyV.txt");
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

		    eliminarRepDV(v);

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