# Hermes
Welcome to my instant messaging application! The main functions of Hermes were designed following a tutorial on Qt provided by the french website openclassrooms. 

Hermes is a server-client based application aimed to connect people from distant places all over the world. The messages are sent by clients on any computer to the server, which in turns send the message back to all the running clients. NEW FEATURE : all messages are now encrypted by my homemade RSA encryption application. Remember the saying though : "Never roll your own crypto, bro". Do not rely on it for security, it's just for fun.


Hermes relies on Qt when it comes to its graphical interface, so that the best way to make it run is compile and launch it on Qt creator, so that
all the DLLs are already installed.



=== SERVER ===
The server interface is minnimalistic. The only useful button changes the RSA keys of the server, so thatany breach in security can be manually prevented in case it became necessary (nope, this is still just for fun). 

=== CLIENT ===
The client GUI has several features :
-> Connect : connects to the server specified by its IP adress and the port used by the server
-> Change keys : once again, changes the user's RSA key. Note both server and client keys are refreshed every minutes. Security definitely matters.
-> Username : choose a funny nickname. You can update it any time, this will not disconnect you from the server.
-> Text message : type in your current mood
-> Send button : mmm what can this be used for?..

=== RSA ===
Hold on, this is the funny part you have been waiting for. this folder contains all the useful functions both server and client use to encrypt/decipher messages. Remember all characters transitting through the network are previously converted to unintelligible numbers before starting their journey.
Those functions need a bunch of prime numbers to run. Here problems begin, since a list of sufficiently enough high valued prime numbers would consist in a way too heavy file for our purpose. This is why I incorporated another of my creation to the project. Let me introduce... JAWAD!

Jawad is a python script that generates prime numbers. It will run as long as you don't stop it, making longer and longer the "liste_premiers.txt" file in the data folder. There, a few other scripts were added to analyse the behaviour of those numbers and Jawad's performances. You have to run the script a little while before the RSA functions begin to be useful. Say, until the list is 300 Mo heavy. Security is worth a few octets.


Have fun using Hermes!

Léonard Lebrun
