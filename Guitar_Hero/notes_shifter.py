



aditivo = input("Intervalo aditivo: ")
multiplicativo = input("Intervalo multiplicativo: ")
f = open("notas_original2.txt")
fw = open("notas.txt", 'w')
for linha in f.readlines():
	time = int(linha.split(':')[1])
	time = int((time + aditivo) * multiplicativo)
	fw.write("%s:%s\n" % (linha.split(':')[0], time))
	#print "%s:%s\n" % (linha.split(':')[0], time)

f.close()
fw.close()
