/* PR tree-optimization/82387 */

struct A { int b; };
int f = 1;

struct A
foo (void)
{
  struct A h[] = { 
    {1}, {1}, {1}, {1}, {1}, {1}, {1}, {1}, {1}, {1}, 
    {1}, {1}, {1}, {1}, {1}, {1}, {1}, {1}, {1}, {1}, 
    {1}, {1}, {1}, {1}, {1}, {1}, {1}, {1}, {1}, {1}, 
    {1}, {1}, {1}, {1}, {1}, {1}, {1}, {1}, {1}, {1}, 
    {1}, {1}, {1}, {1}, {1}, {1}, {1}, {1}, {1}, {1}, 
    {1}, {1}, {1}, {1}, {1}, {1}, {1}, {1}, {1}, {1}, 
    {1}, {1}, {1}, {1}, {1}, {1}, {1}, {1}, {1}, {1}, 
  };
  return h[24];
}

int
main ()
{
  struct A i = foo (), j = i;
  j.b && (f = 0);
  return f; 
}
