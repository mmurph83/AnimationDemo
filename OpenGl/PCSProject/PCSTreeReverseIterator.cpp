#include <assert.h>
#include "PCSTreeReverseIterator.h"


#define STUB_PLEASE_REPLACE(x) (x)

PCSTreeReverseIterator::PCSTreeReverseIterator(PCSNode *rootNode):root(rootNode),current(rootNode),prevNode(nullptr)
{
	STUB_PLEASE_REPLACE(rootNode);
}

PCSNode *PCSTreeReverseIterator::First()
{
	current = root;
	return root;
}

PCSNode *PCSTreeReverseIterator::Next()
{
	prevNode = current;
	if (current != nullptr)
	{
		current = current->GetReverse();
	}
	return current;
}

bool PCSTreeReverseIterator::IsDone()
{

	return (current == nullptr);
}

PCSNode *PCSTreeReverseIterator::CurrentItem()
{
	return current;
}