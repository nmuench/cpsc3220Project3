#include <ios>
#include <iostream>
#include <algorithm>
using namespace std;

struct Pair
{
  int thread_id;
  int thread_priority;
  Pair(){thread_id = 0; thread_priority = 0;}
  Pair(int x, int y){thread_id = x; thread_priority = y;}
};

struct Node
{
  int t;
  int highest;
  Node * l;
  Node * r;
  Node(){t = 0; highest = 0; l = NULL; r = NULL;}
  Node(int x, int h){t = x; highest = h; l = NULL; r = NULL;}
};


Node * insert(Node * T, int x, int k)
{
  if(T == NULL)
  {
    return new Node(x, k);
  }
  if(T->t > x)
  {
    T->l = insert(T->l, x , k);
  }
  else
  {
    T->r = insert(T->r, x, k);
  }
  return T;
}

Node * pred(Node * T, int x)
{
  if(T == NULL)
  {
    return NULL;
  }
  if(T->t == x)
  {
    return T;
  }

  if(T->t > x)
  {
    return pred(T->l, x);
  }
  else
  {
    Node * temp = pred(T->r, x);
    return (temp) ? temp : T;
  }
}


bool operator< (const Pair &a, const Pair &b)
{
  return (a.thread_id < b.thread_id);
}

bool sortPri(const Pair &a, const Pair &b)
{
  return (a.thread_priority > b.thread_priority);
}

void printI(Node * T)
{
  if(T == NULL)
  {
    return;
  }
  printI(T->l);
  cout << T->t << "llll" << endl;
  printI(T->r);
}

int ars[20][2]=		//pairs of thread id and priority
  {{0,0},{1,5},{2,3},{3,4},{4,1},
  {5,10},{6,15},{7,13},{8,14},{9,11},
  {10,0},{11,5},{12,3},{13,4},{14,1},
  {15,10},{16,15},{17,13},{18,14},{19,11}};

int main()
{
  int a = 4;
  int b = 1;

  Node * times = NULL;


  Pair p[20];

  int currTime = 0;


  for(int i = 0; i < 20; i++)
  {
    p[i].thread_id = ars[i][0];
    p[i].thread_priority = ars[i][1];
    //times = insert(times, currTime, i);
    //cout << currTime << endl;
    //currTime += a;
  }



  for(int i = 0; i < 20; i += 4)
  {
    times = insert(times, currTime, i);
    //cout << currTime << endl;
    currTime += a;
  }

  times = insert(times, currTime, 19);
  sort(p, p+20);

  //printI(times);

  currTime = 0;
  int used = 0;

  for(int used = 0; used < 20; )
  {
    sort(p + used, p + 20);
    Node * temp = pred(times, currTime);
    //cout << temp->highest << "|| " << currTime << endl;
    if(temp->highest >= used)
    {
      sort(p + used, p + temp->highest + 1, sortPri);
      cout << p[used].thread_id << endl;
      used++;
    }
    currTime += b;
  }



  return 0;
}
