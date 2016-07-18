#include <stdio.h>
#include <unistd.h> 
#include <iostream>
#include <string>
#include <sstream>
#include <fstream> 
#include <vector>
#include <locale>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <netdb.h>
using namespace std;
void spracuj(string foo,string bar,vector<string> *inf){
  

     string line,prevod,rozdeleno;
     locale loc;
     vector<string> informace;
     char *login;
     
     for (string::size_type i=0; i<bar.length(); ++i){ //prevot vstupu na male pismenka kvuli zadani vstupu nerolisujeme mezi  
	      bar[i]=tolower(bar[i],loc);     		// velkymi a malimi pismeny
	  }
	  
     ifstream soubor("/etc/passwd"); //cteni ze souboru
     
     while(getline(soubor, line)){
	  char * rozdel = new char [line.length()+1];
	  strcpy (rozdel, line.c_str());
	  
	  //rosekam radek na jednotlive informace
	  login = strtok (rozdel,":"); //jednotlive radky jsou odelene :
	    while (login != NULL){
		  prevod=(login);
		  informace.push_back(prevod); //dam je do vektoru
		  login = strtok (NULL, ":");
	    }
	    
	    if(foo=="login") // bud hledam podle loginu 
	    {

	      string porovnej(informace[0]);
	      for (string::size_type i=0; i<porovnej.length(); ++i){
		  porovnej[i]=tolower(porovnej[i],loc);
	      }
	   
	      if(porovnej==bar) inf->insert(inf->end(),informace.begin(),informace.end()); //porovnaj jestli je hledanej pridam dalsipolozky
	      
	    }
	      
	    else //hledam podle uid
	    {
	      if(informace[2]==bar) inf->insert(inf->end(),informace.begin(),informace.end());
	    }
	    
	    informace.clear(); // vymazu kontejner pred dalsim hledanim
	      
    
	}
	if(inf->empty())inf->push_back("nenalezen");
  
}

string najdi(string co,vector<string> *inf){
  
   
     string sprava;
     vector<string> informace;
     char *login;
     int j=0;
     
     for( int t=0; t<inf->size()/7; t++){ 	// kdyz mame vic polozek nez jednu dva stejna uid
      if(t>=1)sprava+="\n"; 			//mezera mezi dvema radkami pokud je jen jeden nerve ji tam
	  for( int i=0; i<co.length(); i++) 	// projizdi co chtel vypsat a rve to do spravy pro klienata
	  {
	    //prida do spravy z vektoru co zrovna potrebujeme
	    if('L' == co[i]) sprava+=(inf->at(j+0)+" ");
	    else if('U' == co[i]) sprava+=(inf->at(j+2)+" ");
	    else if('G' == co[i]) sprava+=(inf->at(j+3)+" ");
	    else if('N' == co[i]) sprava+=(inf->at(j+4)+" ");
	    else if('H' == co[i]) sprava+=(inf->at(j+5)+" ");
	    else if('S' == co[i]) sprava+=(inf->at(j+6)+" ");
	    else sprava+=" ";
	  } 
	  j=j+7; //pricteni pro dalsi radke aby jsme byli ve vektoru na spravnem miste
     }

  return sprava;  
}

using namespace std;
int main (int argc, char **argv) {
  int parametry,cislo_portu=-1,hlavni_soked,soket_prokomunikaci,i;
  bool prijem(true);
  struct sockaddr_in sin;
  char prijmana_sprava[260];
  struct hostent * hp;
  vector<string> informace;
  string sprava,foo,bar,co;
  
  //spracovani parametru
  while ((parametry = getopt(argc, argv, "p:")) != -1) 
    switch (parametry) {  
     
    case 'p':
    cislo_portu=atoi(optarg);
    break;
    
  }
 
  //kontrola parametru
  if (cislo_portu==-1) { 
   cerr << "neni zadane cislo portu "<< endl;  
    return EXIT_FAILURE;
  }
  
  if ( (hlavni_soked = socket(PF_INET, SOCK_STREAM, 0 ) ) < 0) {   //vytvoreni soketu
    cerr << "error on sicked "<< endl;                  
    return EXIT_FAILURE;
  }
  //nastaveni parametru soketu
  sin.sin_family = PF_INET;
  sin.sin_port = htons(cislo_portu);  
  sin.sin_addr.s_addr  = INADDR_ANY;
  
  //presmerovani parametru do hlavniho soketu
  if (bind(hlavni_soked, (struct sockaddr *)&sin, sizeof(sin) ) < 0 ) {
    cerr << "error on bind "<< endl;  
    return EXIT_FAILURE;  
  }
  //naslouchani
  if (listen(hlavni_soked, 5)) { 
    cerr << "error on listen "<< endl;   
    return EXIT_FAILURE;
  }
  
 socklen_t sinlen = sizeof(sin);
  while (1) {

    if ( (soket_prokomunikaci = accept(hlavni_soked, (struct sockaddr *) &sin, &sinlen) ) < 0 ) {
      cerr << "error on accept "<< endl;  //accept error
   
      return EXIT_FAILURE;
    }
    
    //rozdeleni aby obslouzila vic klientu
    int pid = fork();
    if (pid < 0)
    {
          cerr << "error on fork "<< endl;
          return EXIT_FAILURE;
    }
    if (pid == 0)
    {
       close(hlavni_soked); //uzvreni nepotrebujeme musime ho uzavrit v hlavnim jede nadale
	hp=(struct hostent *)gethostbyaddr((char *)&sin.sin_addr,4,AF_INET);

	while(prijem) //zahajeni komunikace pro pripad ze klient pozaduje vice loginu nebo uid
	{
	    bzero(prijmana_sprava,sizeof(prijmana_sprava)); //velikost prijmane spravy
	    
	    if ( read(soket_prokomunikaci, prijmana_sprava, sizeof(prijmana_sprava) ) <0)  //cteni spravy
	    {  
	      cerr << "error on read "<< endl;         
	      return EXIT_FAILURE;
	    }
	  
	    stringstream ifs(prijmana_sprava); //rozdeleni spravy na informace abysme pochopili co chce klient
	    ifs >> foo >>bar>>co ;
	    
	
	  informace.clear(); 
	    spracuj(foo,bar,&informace); //nalezeni konkretni radky do vektoru informace nahraje 7 informaci a danem loginu
	    if(informace.back()=="nenalezen")
	    {
	      if(foo=="login")sprava="Chyba: neznamy login " + bar; // nenalezen sprava pro klienta
	      else sprava="Chyba: neznamy uid " + bar;
	    }
	    else
	    {
	      
		if(informace.size() % 7 == 0 ) // mame sedum informaci proto musi byt nasobky sedmi jinak je chybna vstupni pawssd
		{
		    sprava=najdi(co,&informace); //spracovani informaci jaky chce klient
		    sprava="ok \n"+sprava;
		}
		else sprava="Chyba: chybný formát zdrojové databáze "; // sprava pro klienta ktera mu informuje o chybe
	    }
	    if(foo=="konec") sprava="konec"; //klient poslal spravu o ukonceni komunikace server mu odpovi ze ukoncuje
	    
	    char * prijmana_sprava = new char [sprava.length()+1]; 	//prevedeni stringu na char aby sel poslat
	    strcpy (prijmana_sprava, sprava.c_str()); 			//prekopirovani
	    if ( write(soket_prokomunikaci, prijmana_sprava, strlen(prijmana_sprava) ) < 0 ) //posilani spravy
	    {  
	    cerr << "error on write "<< endl;   
	    return EXIT_FAILURE; 
	    }
	    delete[] prijmana_sprava;
	    if(foo=="konec") prijem=false; //ukonceni vysilani mezi klientem a servrem klient dostal vsechnz data
	}
	;
	if (close(soket_prokomunikaci) < 0) // uzavreni soketu
	{
	    cerr << "error on close "<< endl; 
	    return EXIT_FAILURE; 
	}
	if(foo=="konec") return EXIT_SUCCESS;// ukoneni vlakna
    } 
  }
  if (close(hlavni_soked) < 0) 
  {
   cerr << "error on close "<< endl;; 
    return EXIT_FAILURE; 
  }
  return EXIT_SUCCESS;
}
