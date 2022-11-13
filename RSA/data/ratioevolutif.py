#-*-coding:Latin-1-*

"""Code créant la liste des pourcentages de nombres premiers par tranches de 10k entiers"""

from pickle import Unpickler


with open("liste_premiers","rb") as file:
 u = Unpickler(file)
 meta = u.load()

nb = 0
tranche = 1

with open("ratios_evolutifs.txt","w") as file:
    
 for i,elt in enumerate(meta[0]):
  if elt < tranche * 10000:
   nb+=1
  else:
   ratio = nb/(100*tranche)
   strratio = str(ratio)
   txt = strratio+"\n"
   file.write(txt)
   tranche +=1
