import tty
import sys
import termios
import datetime

intervalo = 240

notas = []
orig_settings = termios.tcgetattr(sys.stdin)

tty.setraw(sys.stdin)
tempo_inicial = datetime.datetime.now()
x = 0
while x != chr(27): # ESC
    x=sys.stdin.read(1)[0]
    if x in "asjkl":
        x = x.replace('a', '0')
        x = x.replace('s', '1')
        x = x.replace('j', '2')
        x = x.replace('k', '3')
        x = x.replace('l', '4')
        time = (datetime.datetime.now() - tempo_inicial).total_seconds()
        notas.append((x, time))
        print "%s:%s" % (x, time)






f = open('notas_original2.txt', 'w')
for nota in notas:
    f.write("%s:%s\n" % (nota[0], int(nota[1]*intervalo)))
f.close()