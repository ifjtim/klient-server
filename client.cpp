#include <stdio.h>
#include <unistd.h> 
#include <iostream>
#include <string>
#include <sstream>
#include <fstream> 
#include <vector>

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
int main (int argc, char **argv) {
  
   vector<string> hledane;
  int parametry,hlavni_soked,  cislo_portu;
  char msgg[400];
  bool L(true),U(true),G(true),N(true),H(true),S(true);
  string sprava,cohledame,podleceho,foo,bar,jmeno;
  struct sockaddr_in sin; struct hostent *hptr;
  char * pch;
  while ((parametry = getopt(argc, argv, "p:h:l:u:LUGNHS")) != -1) 
    switch (parametry) {  
     
    case 'p':
      cislo_portu=atoi(optarg);
    break;
    
    case 'h':
      jmeno=optarg;
    break;
   
    case 'l':

      hledane.clear(); //vymaze predesle zadani bere posledni mozne
      hledane.push_back(string("login ")+optarg);
      while(optind<argc && argv[optind][0] != '-'){ //nhraje vic loginu
	hledane.push_back(string("login ")+argv[optind++]);
      }
    podleceho=string("login ")+optarg;
    break;
    
    case 'u':
       hledane.clear();
       hledane.push_back(string("uid ")+optarg);
       while(optind<argc && argv[optind][0] != '-'){
	    hledane.push_back(string("uid ")+argv[optind++]);
      }
     podleceho=string("uid ")+optarg;
    break;
    
    case 'L':
      if(L)cohledame+=string("L");//nelze zadar dvakrat
      L=false;
    break;
    
    case 'U':
     if(U)cohledame+=string("U");
     U=false;
    break;
    
    case 'G':
      if(G)cohledame+=string("G");
      G=false;
    break;
    
    case 'N':
      if(N)cohledame+=string("N");
      N=false;
    break;
    
    case 'H':
      if(H)cohledame+=string("H");
      H=false;
    break;
    
    case 'S':
      if(S)cohledame+=string("S");
      S=false;
    break;
    
   
  }
  hledane.push_back(string("konec konec konec "));// na konec  se dava sprava o ukonceni
  
  
   if ( (hlavni_soked = socket(PF_INET, SOCK_STREAM, 0 ) ) < 0) //vytvoreni soketu
   { 
    cerr << "error on soket "<< endl;  
      return EXIT_FAILURE;
    }
    //nastaveni parametru
  sin.sin_family = PF_INET;
  sin.sin_port = htons(cislo_portu);  
  char * jmenoservru = new char [jmeno.length()+1]; //prevedeni na char
	strcpy (jmenoservru, jmeno.c_str());
  if ( (hptr =  gethostbyname(jmenoservru) ) == NULL) //nasteveni jmeno servru
  {
    cerr << "error on gethostbyname "<< endl;
    return EXIT_FAILURE;
   }
  memcpy( &sin.sin_addr, hptr->h_addr, hptr->h_length);
  if (connect (hlavni_soked, (struct sockaddr *)&sin, sizeof(sin) ) < 0 )
  {
    cerr << "error on connect "<< endl; 
    return EXIT_FAILURE;
  }
  
  for(int i = 0; i < hledane.size(); i++) //posilani vsech loginu nebo uid postupne
  {
		
	sprava=hledane[i]+string("\n")+cohledame; //spojeni spravy do stringu
	char * msg = new char [sprava.length()+1]; //prevedeni na char
	strcpy (msg, sprava.c_str());
	if ( write(hlavni_soked, msg, strlen(msg) +1) < 0 ) //poslani spravy
	{  
	  cerr << "error on write "<< endl;
	  return EXIT_FAILURE; 
	}
	delete[] msg;
	bzero(msgg,sizeof(msgg)); //velikost prijmane spravy prida nulu
	if ( read(hlavni_soked, msgg, sizeof(msgg)  ) <0) // cist spravu
	{  
	  cerr << "error on ride "<< endl;
	  return EXIT_FAILURE;
	}
	
	stringstream ifs(msgg); //dekodovani chybz nebo naslo
	ifs >> foo  ;
	
	bar= ifs.str(); //prevedeni spravy na string kvuli vypisu chyby
	
	pch = strtok (msgg,"\n"); //odstarneni typu spravy ok 
	  pch = strtok (NULL, "\n"); //prvni radek spravy
	  
	if(foo=="Chyba:")
	{
	  cerr  << bar << endl;    //vypis chyby 
	}
	else if(foo=="ok")
	{
	  while (pch != NULL) //postupne vypisuje radky spravy kdyby bylo vice nalezu nez jeden
	  {
	      cout << pch << endl;
	      pch = strtok (NULL, "\n"); //deli na radky
	  }
	    
	}
	else if(foo=="konec"){;} //pokud prijde sprava o potvrzeni koncu komunikace
	else cerr << "chyba: komunikace "<< endl;  //spatna sprava
	//sleep(2);
  }
 
 delete[] jmenoservru;
  if (close(hlavni_soked) < 0) {  //uzavreni soketu
    cerr << "error on close "<< endl;   /* close error */
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
