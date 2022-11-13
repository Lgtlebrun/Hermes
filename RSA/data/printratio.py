import os


while True:
 print("ATTENTION ne prend en entrée que des multiples de 10^4")
 indice = int(input("Rang?  "))

 while indice%10000 != 0:
  print("Veuillez entrer un multiple de 10 000")
  indice = int(input("Rang?  "))

          
 indice /= 10000
 indice -=1
 indice = int(indice)

 with open("ratios.txt","r") as file:
  ratios = file.read()

 ratios = ratios.split("\n")


 print(ratios[indice])
 os.system("PAUSE")
