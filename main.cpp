#include <cstdio>
#include <iterator>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <cstring>
#include <iostream>
#include "topspin.hpp"
#include "search.hpp"
#include "util.hpp"
#include "getopt_pp.h"

using namespace std;
using namespace GetOpt;

int _n=17,_k=4,_f=9;

void show_help()
{
  return;
}

// TODO:
// add option -a --all to run all instances with the given parameters
int main(int argc, char *argv[])
{

  GetOpt_pp ops(argc, argv);
  // char * nkf=new char[8];
  vector<int> nkf;
  vector<int> state;
  int instance;

  // Just printing the operators that I allow given prev
  if(ops>>OptionPresent('O',"operators")){
    topspin::set_prune(0);	// prunning conditions activated
    topspin dummy(17,4);	// just to get the opers
    int o=0;
    for (int i = 0; i < _n; ++i){
      cout<<i<<": ";
      o=dummy.opers(i);
      for (int j = 0; j < _n; ++j){
	if((1<<j)&o)
	  cout<<j<<" ";
      }
      cout<<endl;
    }
    return 0;
  }

  if(ops>>OptionPresent('h',"help")){
    show_help();
    return 0;
  }

  if(ops>>OptionPresent('v',"verbose")){
    topspin::__verb__=1;
  }

  if(ops>>Option('B',"build-implicit",nkf)){
    cout<<"creating pdb: "<<nkf[0]<<nkf[1]<<nkf[2]<<endl;
    _n=nkf[0];
    _k=nkf[1];
    _f=nkf[2];

    // memory to be reserved
    int ent=1;
    for (int i = 1; i < _f; ++i)
      ent*=(_n-i);
    
    char * pdb = (char *)malloc(ent);
    memset(pdb,-1,ent);

    tspattern gp(_n,_k,_f);
    cout<<gp<<endl;

    cout<<"size of pdb:\t"<<ent<<endl;
    cout<<"entries computed:\t"<<implicitbfs<tspattern>(gp,pdb,ent)<<endl;

    stringstream fname;
    fname<<"pdbs/"<<(int)_n<<"-"<<(int)_k<<"-"<<_f<<".pdb";
    ofstream of(fname.str().c_str());
    of.write(pdb,ent);
    cout<<"output file:\t"<<fname.str()<<endl;
    if(of.bad())
      cout<<"\tPDB bad!!!"<<endl;
    of.close();
    
    return 0;
  }

  if(ops>>Option('b',"build-pdb",nkf)){
    cout<<"creating pdb: "<<nkf[0]<<nkf[1]<<nkf[2]<<endl;
    _n=nkf[0];
    _k=nkf[1];
    _f=nkf[2];

    // memory to be reserved
    int ent=1;
    for (int i = 1; i < _f; ++i)
      ent*=(_n-i);
    
    char * pdb = (char *)malloc(ent);
    memset(pdb,0,ent);
    tspattern gp(_n,_k,_f);
    cout<<gp<<endl;
    cout<<"size of pdb:\t"<<ent<<endl;
    cout<<"entries computed:\t"<<pdbgen<tspattern>(gp,pdb)<<endl;
    stringstream fname;
    fname<<"pdbs/"<<(int)_n<<"-"<<(int)_k<<"-"<<_f<<".pdb";
    ofstream of(fname.str().c_str());
    of.write(pdb,ent);
    cout<<"output file:\t"<<fname.str()<<endl;
    if(of.bad())
      cout<<"\tPDB bad!!!"<<endl;
    of.close();

    // freeing memory
    free(pdb);

    return 0;
  }
  
  // loading pdb if specified
  string pdbfname;
  if(ops>>Option('f',"fringe",_f)>>Option('p',"load-pdb",pdbfname)){
    char * pdb;
    loadpdb(pdbfname.c_str(),&pdb);
    topspin::set_pdb(pdb,_f);
    #ifdef VERB
    cout<<"fringe:"<<_f<<endl;
    #endif
  }

  // prepare state
  ops>>Option('n',_n);
  ops>>Option('k',_k);
  topspin ts(_n,_k);

  // multiple lookups
  short multi=0;
  if(ops>>Option('m',"multi",multi))
    topspin::set_multi(multi);

  // problem-specific prunning
  char prune=0;
  if(ops>>OptionPresent('u',"prune")){
    topspin::set_prune(1);
    prune=1;
  }

  // symmetry
  vector<int> syms;
  if(ops>>Option('y',"syms",syms))
    topspin::init_sym(syms);

  // Duality
  short _dual=0;
  if(ops>>OptionPresent('d',"dual")){
    topspin::set_dual(1);
    _dual=1;
  }

  // BPMX
  if(ops>>OptionPresent('x',"bpmx"))
    topspin::bpmx=1;
    

  // reading state
  if(ops>>Option('i',"instance",instance)){
    ts.init(topspin::instances[instance]);
  }else if(ops>>OptionPresent('a',"all")){ // solving all 1000 instances
    short C=0;				   // C*
    time_t seconds;
    for (int i = 0; i < 1000; ++i){	   // 100 for 18-4 and 1000 for 17-4
      seconds=time(NULL);		   // start timer
      ts.init(topspin::instances[i]);
      printf("-i %d -k %d -f %d -p %s ",i,_k,_f,pdbfname.c_str());

      if(syms.size()) {		// -y
	cout<<"-y "; 
	copy(syms.begin(),syms.end(),ostream_iterator<int>(cout," "));
      }
      if(_dual) printf("-d ");	       // -d
      if(topspin::bpmx) printf("-x ");  // -x
      if(multi) printf("-m %d ",multi); // -m
      if(prune) printf("-u ");		// -u

      printf("\n");

      // choose algorithm
      if(ops>>OptionPresent('D',"dualsearch")){
	C=didastar<topspin>(ts);
      }else {
	C=idastar<topspin>(ts);
      }
      // print results
      cout<<"C*:"<<C<<" ";
      cout<<"exp:"<<hs::exp<<" ";
      cout<<"gen:"<<hs::gen<<" ";
      cout<<"time:"<<time(NULL)-seconds<<" ";
      cout<<"sol: ";
      copy(hs::soln.begin(), hs::soln.end(), ostream_iterator<short>(cout," "));
      cout<<endl;
    }
    return 0;
  }

  if(ops>>Option('s',"state",state)){
    ts.init(state);
  }

  cout<<"state: "<<ts<<endl;

  // unrank only
  vector<unsigned> u;
  if(ops>>Option('R',"unrank",u)){
    tspattern::init(u[1],u[2]);	// (n, f)
    tspattern tsp(u[0]);
    cout<<tsp<<endl;
    
    return 0;
  }

  // rank only
  if(ops>>OptionPresent('r',"rank")){
    ops>>Option('f',"fringe",_f);
    topspin::set_fringe(_f);
    if(ops>>Option('m',"multi",multi));

    cout<<"Indices:"<<endl;
    for (int i = 0; i <= multi; ++i){
      cout<<"hashval("<<i<<"): "<<ts.hashval(i)<<endl;	
    }
    if(syms.size()){
      cout<<"Sym indices:"<<endl;
      ts.init_sym(syms);		// assume reverse, look at topspin.hpp
      ts.sym();
      // printf("D(%d,r=%d)=",i,r);
      // copy(topspin::m_D,topspin::m_D+ts.m_n,ostream_iterator<int>(cout," "));
      // cout<<endl<<"s'="<<ts<<endl;
      // printing hashvals
      // cout<<" hashvals(s)={";
      for (int i = 0; i <= multi; ++i){
	cout<<"hashval("<<i<<"): "<<ts.hashval(i)<<endl;
      }
    }
    
    if(ops>>OptionPresent('d',"dual")){
      cout<<"Dual indices:"<<endl;
      ts.dual();		// get dual state
      for (int i = 0; i <= multi; ++i){
	cout<<"hashval("<<i<<"): "<<ts.hashval(i)<<endl;	
      }
      if(syms.size()){
	cout<<"Sym indices:"<<endl;
	ts.init_sym(syms);		// assume reverse, look at topspin.h
	ts.sym();
	// printf("D(%d,r=%d)=",i,r);
	// copy(topspin::m_D,topspin::m_D+ts.m_n,ostream_iterator<int>(cout," "));
	// cout<<endl<<"s'="<<ts<<endl;
	// printing hashvals
	// cout<<" hashvals(s)={";
	for (int i = 0; i <= multi; ++i){
	  cout<<"hashval("<<i<<"):"<<" "<<ts.hashval(i)<<endl;
	}
      }
    }
    return 0;
  }
  

  // random walk
  int steps;
  if(ops>>Option('w',"walk",steps)){ // n and k have been specified at this point
    ts.randomwalk(steps);	// ts is the initial state created above
    cout<<ts<<endl;
    return 0;
  }

  // choose algorithm to run
  short C;
  if(ops>>OptionPresent('D',"dualsearch")){
    C=didastar<topspin>(ts);
  }else {
    C=idastar<topspin>(ts);
  }

  cout<<"C*:"<<C<<endl;
  cout<<"exp:"<<hs::exp<<endl;
  cout<<"gen:"<<hs::gen<<endl;
  cout<<"sol: ";
  copy(hs::soln.begin(), hs::soln.end(), ostream_iterator<short>(cout," "));
  cout<<endl;
 
  return 0;
}
