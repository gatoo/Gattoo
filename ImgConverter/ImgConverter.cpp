// ImgConverter.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <list>
#include <iostream>
#include <time.h>

template <class T>
void GetAllCombos(std::list<T> &initialList, std::list<std::list<T> > & outList)
{
	// The final number of sets will be 2^N (or 2^N - 1 if skipping empty set)
	size_t setCount = (size_t) pow(2.0, (int)initialList.size());

	std::list<T>::const_iterator iter;

	// Start at 1 if you do not want the empty set
	for (size_t mask = 0; mask < setCount; mask++)
	{
		std::list<T> nestedList;
		size_t j ;

		for (j= 0, iter = initialList.begin(); iter != initialList.end(); j++, ++iter)
		{
			// Each position in the initial list maps to a bit here
			size_t pos = 1 << j;
			if ((mask & pos) == pos) { nestedList.push_back(*iter); }
		}

		outList.push_back(nestedList);
	}
}

template <class T>
void Combine(std::list<T> &initialList, typename std::list<T>::reverse_iterator initialListIter, std::list<std::list<T> > & outList)
{
	if (initialList.rend() == initialListIter) return;
	
	std::list<std::list<T> >::const_iterator iterStop = outList.insert(outList.end(), std::list<T>(1, *initialListIter));
	std::list<std::list<T> >::const_iterator iter = outList.begin();

	for(iter; iter != iterStop; ++iter)
	{
		(*outList.insert(outList.end(), *iter)).push_front(*initialListIter);
	}

	Combine(initialList, ++initialListIter, outList);
}

typedef std::list<std::list<char> > CResultList;
typedef std::list<char> CElementList;

int _tmain(int argc, _TCHAR* argv[])
{
	CElementList initial;
	CResultList outList;

	int const testSetSize = 25;

	for(int i = 0; i<testSetSize; i++)
		initial.push_back('A' + i);
	
	time_t tStart, tStop;

	std::cout << "Processing started" << std::endl;
	time(&tStart);

	//GetAllCombos(initial, outList);

	Combine(initial, initial.rbegin(), outList);

	time(&tStop);

	if (testSetSize < 10)
	{
		CResultList::const_iterator iterRes;
		CElementList::const_iterator iterElem;

		for (iterRes = outList.begin(); iterRes != outList.end(); ++iterRes)
		{
			for (iterElem = (*iterRes).begin(); iterElem != (*iterRes).end(); ++iterElem)
			{
				std::cout << *iterElem;
			}
			std::cout << std::endl;
		}
	}
	
	std::cout << "There are " << outList.size() << " combinations in result list" << std::endl;
	std::cout << "Processing finished in " << difftime(tStop, tStart) << " seconds" << std::endl;

	return 0;
}

