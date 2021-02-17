practica3alg:
	echo "Los casos de ejecución serán descomprimidos en la carpeta bin"
	g++ -std=c++11 ./src/algoritmos_comunes.cpp -o ./bin/algoritmos_comunes
	g++ -std=c++11 ./src/optimizarcontenedores.cpp -o ./bin/optimizarcontenedores
	g++ -std=c++11 ./src/optimizartoneladas.cpp -o ./bin/optimizartoneladas
	gunzip -k ./data/*.tsp.gz 
	mv ./data/*.tsp ./bin/

mrproper:
	rm ./bin/*.tsp

clean:
	rm ./bin/*

