from pickle import *

with open("liste_premiers","rb") as file:
	u = Unpickler(file)
	meta = u.load()

txt = " "
for elt in meta:
    for i, nombre in enumerate(elt):
        elt[i] = str(elt[i])


    txt += ", ".join(elt)


txt = txt.strip()

with open("liste_premiers.txt", "w") as file:
 file.write(txt)
