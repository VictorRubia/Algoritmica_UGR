practica4alg:
	echo "Los casos de ejecución serán descomprimidos en la carpeta bin"
	g++ -std=c++11 ./src/programaciondinamica.cpp -o ./bin/programaciondinamica
	gunzip -k ./data/*.tsp.gz 
	mv ./data/*.tsp ./bin/

mrproper:
	rm ./bin/*.tsp

clean:
	rm ./bin/*

