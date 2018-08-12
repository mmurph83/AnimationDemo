#include <assert.h>
#include "PCSTreeForwardIterator.h"


#define STUB_PLEASE_REPLACE(x) (x)

PCSTreeForwardIterator::PCSTreeForwardIterator(PCSNode *rootNode):root(rootNode),current(rootNode)
{
	STUB_PLEASE_REPLACE(rootNode);
}

PCSNode *PCSTreeForwardIterator::First()
{

	return root;
}

PCSNode *PCSTreeForwardIterator::Next()
{
	if (current != nullptr)
	{
		current = current->GetForward();
	}
	return current;
}


bool PCSTreeForwardIterator::IsDone()
{

	return (current == nullptr);
}

PCSNode *PCSTreeForwardIterator::CurrentItem()
{
	return current;
}
