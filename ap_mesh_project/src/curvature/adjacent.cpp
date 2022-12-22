#include "../../include/curvature/adjacent.h"

extern double TOLERANCIA;

// P de Primeiro, U de Último !!! :D
// confirma que U é adjacente à esquerda de P
bool Adjacent::ConfirmLeftAdjacency(const Noh& noh,
                                    const Elemento& first_element,
                                    const Elemento& last_element) {
  //	quem é o noh para P? ( n1, n2 ou n3? )
  //	se noh = P.n1 então verifique se U possui a sequência P.n1 P.n3

  if (noh == first_element.getN(1)) {
    if (((first_element.getN(1) == last_element.getN(1)) and
         (first_element.getN(3) == last_element.getN(2))) or

        ((first_element.getN(1) == last_element.getN(3)) and
         (first_element.getN(3) == last_element.getN(1))) or

        ((first_element.getN(1) == last_element.getN(2)) and
         (first_element.getN(3) == last_element.getN(3))))
      return true;
  }

  //	se noh = P.n2 então verifique se U possui a sequência P.n2 P.n1
  if (noh == first_element.getN(2)) {
    if (((first_element.getN(2) == last_element.getN(1)) and
         (first_element.getN(1) == last_element.getN(2))) or

        ((first_element.getN(2) == last_element.getN(3)) and
         (first_element.getN(1) == last_element.getN(1))) or

        ((first_element.getN(2) == last_element.getN(2)) and
         (first_element.getN(1) == last_element.getN(3))))
      return true;
  }

  //	se noh = P.n3 então verifique se U possui a sequência P.n3 P.n2
  if (noh == first_element.getN(3)) {
    if (((first_element.getN(3) == last_element.getN(1)) and
         (first_element.getN(2) == last_element.getN(2))) or

        ((first_element.getN(3) == last_element.getN(3)) and
         (first_element.getN(2) == last_element.getN(1))) or

        ((first_element.getN(3) == last_element.getN(2)) and
         (first_element.getN(2) == last_element.getN(3))))
      return true;
  }

  return false;
}

// confirma que U é adjacente à direita de P
bool Adjacent::ConfirmRightAdjacency(const Noh& noh,
                                     const Elemento& first_element,
                                     const Elemento& last_element) {
  //	quem é o noh para P? ( n1, n2 ou n3? )
  //	se noh = P.n1 então verifique se U possui a sequência P.n2 P.n1
  if (noh == first_element.getN(1)) {
    if (((last_element.getN(1) == first_element.getN(1)) and
         (last_element.getN(3) == first_element.getN(2))) or

        ((last_element.getN(3) == first_element.getN(1)) and
         (last_element.getN(2) == first_element.getN(2))) or

        ((last_element.getN(2) == first_element.getN(1)) and
         (last_element.getN(1) == first_element.getN(2))))
      return true;
  }

  //	se noh = P.n2 então verifique se U possui a sequência P.n3 P.n2
  if (noh == first_element.getN(2)) {
    if (((last_element.getN(1) == first_element.getN(2)) and
         (last_element.getN(3) == first_element.getN(3))) or

        ((last_element.getN(3) == first_element.getN(2)) and
         (last_element.getN(2) == first_element.getN(3))) or

        ((last_element.getN(2) == first_element.getN(2)) and
         (last_element.getN(1) == first_element.getN(3))))
      return true;
  }

  //	se noh = P.n3 então verifique se U possui a sequência P.n1 P.n3
  if (noh == first_element.getN(3)) {
    if (((last_element.getN(1) == first_element.getN(3)) and
         (last_element.getN(3) == first_element.getN(1))) or

        ((last_element.getN(3) == first_element.getN(3)) and
         (last_element.getN(2) == first_element.getN(1))) or

        ((last_element.getN(2) == first_element.getN(3)) and
         (last_element.getN(1) == first_element.getN(1))))
      return true;
  }

  return false;
}

Elemento* Adjacent::GetElementLeft(const Noh& noh, Elemento* element,
                                   list<Elemento*>& list_element) {
  Elemento* element_result = nullptr;
  list<Elemento*>::iterator element_iterator;

  for (element_iterator = list_element.begin();
       element_iterator != list_element.end(); ++element_iterator) {
    if (Adjacent::ConfirmLeftAdjacency(noh, *element, *(*element_iterator))) {
      element_result = *element_iterator;
      list_element.erase(element_iterator);
      break;
    }
  }

  return element_result;
}

// busca na lista um elemento adjacente à direita de elem
Elemento* Adjacent::GetElementRight(const Noh& noh, Elemento* element,
                                    list<Elemento*>& list_element) {
  Elemento* element_result = nullptr;
  list<Elemento*>::iterator element_iterator;

  for (element_iterator = list_element.begin();
       element_iterator != list_element.end(); ++element_iterator) {
    if (Adjacent::ConfirmRightAdjacency(noh, *element, *(*element_iterator))) {
      element_result = *element_iterator;
      list_element.erase(element_iterator);
      break;
    }
  }

  return element_result;
}

// verifica a concavidade de dois elementos adjacentes
int Adjacent::ConcavityElement(const Noh& noh, const Elemento& first_element,
                               const Elemento& next_element) {
  Vetor vector;

  if (noh == next_element.getN(1))
    vector = Vetor(next_element.getN(1), next_element.getN(3));
  else if (noh == next_element.getN(2))
    vector = Vetor(next_element.getN(2), next_element.getN(1));
  else
    vector = Vetor(next_element.getN(3), next_element.getN(2));

  double d;
  d = first_element.getNormal() ^ vector;

  if (fabs(d) <= TOLERANCIA) return 0;

  if (d > 0)
    return -1;
  else
    return 1;
}

double Adjacent::AngleElement(const Elemento& first_element,
                              const Elemento& next_element) {
  Vetor first_vector = first_element.getNormal();
  Vetor next_vector = next_element.getNormal();

  return first_vector.anguloCom(next_vector);
}
