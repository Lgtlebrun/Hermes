#-*-coding:Latin-1-*

from pickle import Unpickler
import os

with open("liste_premiers","rb") as file:
 u = Unpickler(file)
 meta = u.load()


i = 0
nb = 0

while meta[0][i] < 10000:
    nb+=1
    i+=1


print(nb/100)
os.system("PAUSE")
