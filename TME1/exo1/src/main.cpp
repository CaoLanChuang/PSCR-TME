#include "List.h"
#include <string>
#include <iostream>
#include <cstring>
#include "String.h"


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

	if (it == nullptr)
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


size_t String::length(const char* s)
{
    size_t i = 0;
    while (s[i]) {
        i++;
    }
    return i;
}

char *String::newcopy(const char* s)
{
    char* str2 = (char*)malloc((length(s)+1)*(sizeof(char)));
	for (int i = 0; i <= length(s); i++)
	{
		str2[i] = s[i];
	}

    return str2;
}



String::String(const char *s)
{	
    str = newcopy(s);
}

String::~String()
{
    delete[] str;
}

String::String(const String &other)
{
    str = newcopy(other.str);
}

String &String::operator=(const String &other)
{
    if (this != &other) 
	{
    	delete[] str;
        str = newcopy(other.str);
    }
    return *this;
}



std::ostream &operator<<(std::ostream &os, const String &s)
{
    os << "String Data : ";
    os << s.str << std::endl;
    return os;
}



int String::compare(const String *other) const
{
    const char* s1 = this->str;
    const char* s2 = other->str;

    while (*s1 != '\0' && (*s1 == *s2)) {
        ++s1;
        ++s2;
    }

    return (*s1 - *s2);
}

bool String::operator<(const String &b) const
{
    return  this->compare(&b) < 0;
}

bool operator==(const String &a, const String &b)
{
    return a.compare(&b) == 0;
}









int main () {

	//Question 4 ~ 7
	int tab[10];
	for (int i = 0 ; i  < 10; i ++)
	{
		tab[i] = i;
	}

	for (int i=9; i >= 0 ; i--) {
		if (tab[i] - tab[i-1] != 1) {
			std::cout << "probleme !";
		}
	}
	//Fault : size_t est unsigned, il ne pas etre negatif !
	
	

	//Question 8
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

	//FAULT : Pour les tableaux, nous devrions utiliser delete[] au lieu de delete.


	/*for (char *cp = str ; *cp ; cp++) {	
		delete cp;
	}*/
	
	// et la chaine elle meme
	//delete str;

	delete[] str;
	str = nullptr;

	

	//Question 9 ~ 12
	String* a = new String("abc");
	String b("bcd");
	String* c(a);
	String* d = new String("bcd");


	std::cout << *a << std::endl;
	std::cout << b << std::endl;
	std::cout << *c << std::endl;

	if (a==c) 
		std::cout<<"Yes!\n"<<std::endl;
	else
		std::cout<<"No!\n"<<std::endl;

	if (a==d) 
		std::cout<<"Yes!\n"<<std::endl;
	else
		std::cout<<"No!\n"<<std::endl;

	std::cout <<"Before = : " << *d << std::endl;
	d = a;
	std::cout <<"After = : " << *d << std::endl;
	delete a;
	delete c;
	delete d;

	std::cout <<"Good Bye" << std::endl;

}
