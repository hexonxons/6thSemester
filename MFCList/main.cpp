#include <iostream>
#include <sstream>
#include <cstdio>
#include "list.h"
int main()
{
	data elem;
	CBasicDataBase<data> dataBase;
	int j = 0;
	for(int i = 0; i < 29; ++i)
	{
		std::ostringstream os;
		os << i << "`th element" << '\0';
		elem.s = os.str();
		elem.num = j;
		j++;
		dataBase.Add(elem);
	}
	printf("");
	for(CBasicDataBase<data>::Iterator i = dataBase.Begin(); i.isValid(); i.Next())
	{
		data *p = i.Current();
		string s = p->s;
		int num = p->num;
		fprintf(stdout, "%d, %s\r\n", num, s.c_str());
	}
	return 0;
}