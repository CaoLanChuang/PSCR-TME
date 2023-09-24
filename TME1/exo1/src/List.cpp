#include <stddef.h>		//Fault : nullptr est define dans cette lib
#include "List.h"
namespace pr {

// ******************* Chainon
Chainon::Chainon (const std::string & data, Chainon * next):data(data),next(next) {};

size_t Chainon::length() {
	size_t len = 1;
	if (next != nullptr) {
		len += next->length();
	}
	return len;	//Faut : si retuen length(), il est un recursive et il va cycle sans fin
}

void Chainon::print (std::ostream & os) const {		//Fault : Le nom de la fonction dans le fichier cpp doit être cohérent avec le nom de la fonction dans le fichier h.
	os << data ;
	if (next != nullptr) {
		os << ", ";
		next->print(os);		//Fault : Si cette ligne de code est placée en dehors du jugement if, il sera bloquée sur le pointeur nul et provoquera la fin anormale du programme.
	}
}

// ******************  List
const std::string & List::operator[] (size_t index) const  {
	Chainon * it = tete;
	
	for (size_t i=0; i < index ; i++) {
		it = it->next;
	}

	if (it == nullptr)			//Fault : Eviter que si index est > length ou pas !
	{
		throw std::out_of_range("Out of Range !\n");
	}
	else	
		return it->data;
	
}

void List::push_back (const std::string& val) {
	if (tete == nullptr) {
		tete = new Chainon(val);
	} else {
		Chainon * fin = tete;
		while (fin->next) {
			fin = fin->next;
		}
		fin->next = new Chainon(val);
	}
}

void List::push_front (const std::string& val) {
	tete = new Chainon(val,tete);
}

bool List::empty() {			//Fault : Si on n'écrit pas List ::, il ne sera pas reconnu
	return tete == nullptr;
}

size_t List::size() const {
	if (tete == nullptr) {
		return 0;
	} else {
		return tete->length();
	}
}

std::ostream & operator<< (std::ostream & os, const pr::List & vec)		//Fault : Cette fonction fait partie de l'espace de noms et ne doit pas être définie en dehors
{
	os << "[";
	if (vec.tete != nullptr) {
		vec.tete->print (os) ;
	}
	os << "]";
	return os;
}

} // namespace pr



