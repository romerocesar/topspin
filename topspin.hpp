#ifndef _TOPSPIN_H_
#define _TOPSPIN_H_

#include <climits>
#include <cstdlib>
#include <algorithm>
#include <iostream>
#include <vector>
#include <utility>

#include "search.hpp"

using namespace std;

class topspin
{
protected:
  char * m_tokens;
  pair<char,char> m_target;	// where to jump. first=i. second=sym|dual
  char m_h;			// heuristic for bpmx
  static char m_k;
  static char m_prune;
  static char m_f;
  static char * m_pdb;
  static char m_multi;
  static char m_dual;
public:
  static char m_jump;
  static char bpmx;
  static char instances[][17];
  // sym variables
  static char m_sym;
  static char * m_D;
  static char * m_E;
  static vector<symmetry> m_syms;

  static char __verb__;
  static char m_n;
  // constructors
  topspin(const char n, const char k);
  topspin(const vector<int>& s, const char k);
  topspin(const topspin& cp);
  // destructor
  virtual ~topspin();
  // functions
  static void init(const size_t n, const size_t k){m_n=n;m_k=k;};
  void init(const char * a);
  void init(const vector<int>& a);
  int move(int i);
  int opers(short prev=0xffff) const;
  static int inv(int i){return i;};
  int goal() const;
  void randomwalk(size_t steps=50);
  void print(char first=0) const;
  // heuristic function
  int hashval(char first=0) const;
  short h(short H=SHRT_MAX);
  // symmetry functions
  void sym(int sym=0);
  static void init_sym(vector<int> syms, char reverse=1);	// assume reverse because standard doesn't add anything on top of the normal renumbering... 
  void dual();
  static void set_dual(char d){m_dual=d;};
  bool jump(bool side, short & prev);
  // seters
  static void set_pdb(char * pdb, char fringe);
  static void set_multi(char m){m_multi=m;}
  static void set_fringe(char fringe){m_f=fringe;}
  static void set_prune(char prune){m_prune=prune;}
  // interactive mode
  void play();

  // operators
  char operator[](size_t i) const;
  friend ostream& operator<<(ostream& out, const topspin& ts);
};

class tspattern: public topspin
{
public:
  tspattern(char N, char K, char f);
  tspattern(const unsigned r, const char n, const char k, const char f);
  tspattern(const unsigned r);
  tspattern(const tspattern& cp);
  virtual ~tspattern();
  // functions
  static void init(const char n, const char k, const char f){m_n=n;m_k=k;m_f=f;};
  static void init(const char n, const char f){m_n=n;m_f=f;};
  int goal(){return 0;}
};


#endif /* _TOPSPIN_H_ */
