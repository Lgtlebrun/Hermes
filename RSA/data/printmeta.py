from pickle import *
import os

indice = int(input("Rang?  "))-1

with open("liste_premiers","rb") as file:
	u = Unpickler(file)
	meta = u.load()

print(meta[0])
os.system("PAUSE")
