Archivo_entrada = 100k.csv
Archivo_salida = 100k.txt
Cantidad_discos = 20
Ancho_discos = 40

All:
	gcc lab1.c -lm -o lab1
	gcc vis.c -lm -o vis

	./lab1 -b -i $(Archivo_entrada) -o $(Archivo_salida) -n $(Cantidad_discos) -d $(Ancho_discos)
