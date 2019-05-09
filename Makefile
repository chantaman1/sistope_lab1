Archivo_entrada = prueba2.csv
Archivo_salida = out1.txt
Cantidad_discos = 1
Ancho_discos = 60

All:
	gcc -o lab1 lab1.c -lm
	gcc -o vis vis.c -lm 

	./lab1 -i $(Archivo_entrada) -o $(Archivo_salida) -n $(Cantidad_discos) -d $(Ancho_discos)