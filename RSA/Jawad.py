#-*-coding:Latin-1-*
#Hosannah dans le 7ème ciel

from math import *
from pickle import *
import time
from threading import Thread


path = r"data\liste_premiers"
chrono = 0



def Lpremiers(metaliste):

 """Fonction effectuant la recherche de tous les nombres premiers inférieurs à la moitié de a"""

 
 indice = 0
 global chrono
 chrono = time.time()
 
 
 a = 33931086844518982011982560935885732032396635556994207701963662088123265314176330336254535971207181169698868584991941607780111073928236261199604691797570505851011072000000000000000000000000001
 #On rappelle que a est égal à (116! + 1)
 racine_a = sqrt(a/2)
 

 nombre = metaliste[-1][-1]

 while nombre < racine_a:   #Pour vérifier si a est premier, nul besoin d'aller plus loin que sa racine

   
  premier = True
  casser = False  #variable pour casser la recherche
  for lpremiers in metaliste:
   for elt in lpremiers:
   
    if nombre % elt == 0:   #On casse la recherche si on trouve un diviseur du nombre
        premier = False
        casser = True
        break
    
    if elt > sqrt(nombre):    #On casse la recherche après avoir tapé la racine du nombre
        casser = True
        break
    
   if casser == True:
       break
    


  if premier:
   indice += 1
   indice = enregistre(metaliste,nombre,indice)
     

  nombre += 1


#Fin de la fonction Lpremiers

  


def enregistre(metaliste,nombre,indice):
 """ Fonction qui enregistre le nombre premier dans la liste appropriée"""

 global path
 global chrono

 
 
 metaliste[-1].append(nombre) #On enregistre le nombre au bout de la dernière liste de la metaliste






 with open(r"data\liste_premiers.txt","a") as file:
      file.write(", "+ str(metaliste[-1][-1])) #On met à jour le fichier texte





 if indice%10000 == 0: #On enregistre la liste tous les 10000 nombres

  temps_f = time.time() #On enregistre le temps d'arrivée
  
  print("Enregistrement...\n")
  print("Dernier Premier en date: ",metaliste[-1][-1])

  taille = 0

  
  
  for elt in metaliste: #On compte tous les éléments de la métaliste
   taille += len(elt)
  print("Nous avons identifié ", taille, " nombres premiers à ce jour")

  proportion = (taille/nombre) *100
  print("Proportion: ", proportion, "%\n\n")
  
  with open("data/ratios_evolutifs.txt","a") as file_:
      txt = str(proportion)+"\n"
      file_.write(txt)

  
  if chrono != 0:   #Condition pour éviter l'enregistrement de la vitesse au premier passage

   vitesse = 10000/(temps_f - chrono)   
   save_vitesse(vitesse)

   chrono = time.time()
   
  else:                 
      chrono = time.time()  #On démarre le chrono au premier passage
     
  
  with open(path,"wb") as file: #On actualise la liste binaire des premiers
   p = Pickler(file)
   p.dump(metaliste)





 if len(metaliste[-1]) > 500000000: #Gestion des capacités maximales des listes
     print("Liste pleine. Création d'une nouvelle liste...")
     print("N'éteignez pas l'algorithme...")
     metaliste.append(list())

     
 
 return indice
    
#Fin de la fonction enregistre()







def save_vitesse(vitesse):
    """Fonction enregistrant les vitesses moyennes dans un fichier pour analyse"""
    
    with open("data/vitesse.txt","a") as file:
        txt = str(vitesse) + "\n"
        file.write(txt)



def save_ratio(nombre):  
    """Fonction enregistrant le ratio de nombres premiers apr tranche dans un fichier

ATTENTION : ne sera utilisable que lorsqu'on aura un pointeur sur la variable "nombre"
afin d'appeler la fonction tous les 10 000 nombres. Pas compatible avec Lpremiers"""
    
    with open("data/ratios.txt","a") as file:
        txt = str(nombre/100) + "\n"
        file.write(txt)


               
    
    
#Main


try:
 with open(path,"rb") as file:
      u = Unpickler(file)
      metaliste = u.load()
except FileNotFoundError:
    metaliste = [[2]]

print("Dernier Premier en date: ",metaliste[-1][-1])

Lpremiers(metaliste)








 

