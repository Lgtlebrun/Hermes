
def doublons():
 try:
  with open("data/liste_premiers.txt","r") as file:
   txt = file.read()
 except FileNotFoundError:
     with open("data/liste_premiers.txt","r") as file:
         txt = file.read()
        
 liste = txt.split(", ")

 taille = len(liste)

 indices=list()


 j=0

 for i in range(taille-1):
  if liste[i] == liste[i+1]:
   j+=1
   indices.append(i)

  

 delta = 1
 for elt in indices:
     del(liste[elt+delta])
     delta-=1


 with open("data/liste_premiers.txt","w") as file:
     txt = ", ".join(liste)
     file.write(txt)


 print("La liste comportait ",taille, " éléments dont ",j, " doublons")

 err = (j/taille) *100
 nb = taille - j

 print("Soit une erreur relative de ", err, "%\n")
 print("On a quand même ", nb, " nombres premiers avec")



