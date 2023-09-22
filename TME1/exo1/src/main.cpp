#include "List.h"
#include <string>
#include <iostream>
#include <cstring>


#include <stddef.h>
//#include "List.h"
namespace pr {

// ******************* Chainon
Chainon::Chainon (const std::string & data, Chainon * next):data(data),next(next) {};

size_t Chainon::length() {
	size_t len = 1;
	if (next != nullptr) {
		len += next->length();
	}
	return len;
}

void Chainon::print (std::ostream & os) const {
	os << data ;
	if (next != nullptr) {
		os << ", ";
		next->print(os);
	}
	
}

// ******************  List
const std::string & List::operator[] (size_t index) const  {
	Chainon * it = tete;
	for (size_t i=0; i < index ; i++) {
		it = it->next;
	}
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

bool List::empty() {
	return tete == nullptr;
}

size_t List::size() const {
	if (tete == nullptr) {
		return 0;
	} else {
		return tete->length();
	}
}

std::ostream & operator<< (std::ostream & os, const pr::List & vec)
{
	os << "[";
	if (vec.tete != nullptr) {
		vec.tete->print (os) ;
	}

	os << "]";
	return os;
}

} // namespace pr


int main () {

	std::string abc = "abc";
	char * str = new char [3];
	str[0] = 'a';
	str[1] = 'b';
	str[2] = 'c';
	size_t i = 0;

	if (! strcmp (str, abc.c_str())) {
		std::cout << "Equal !";
	}

	pr::List list;
	


	list.push_front(abc);
	
	list.push_front(abc);

	std::cout << "Liste : " << list << std::endl;
	
	std::cout << "Taille : " << list.size() << std::endl;

	
	// Affiche à l'envers
	for (i = list.size() ; i > 0; i--)	//Fault  : unsigned long long cant be negative
	{
		std::cout << "elt " << i - 1<< ": " << list[i-1] << std::endl;
	}

	// liberer les char de la chaine
	for (char *cp = str ; *cp ; cp++) {
		delete cp;
	}
	// et la chaine elle meme
	delete str;

}
