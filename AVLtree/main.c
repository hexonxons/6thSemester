#include <stdio.h>
#include <stdlib.h>
#include "avlTree.h"

//#define rootNode struct rootNode
//#define nodeData struct nodeData


int main(int argc, char **argv)
{
	struct treeNode *root;
	struct nodeData data;

	data.key = 5;
	data.str = 5;
	root = initTree(data);

	data.key = 15;
	data.str = 15;
	root = insertNode(root, data);

	data.key = 17;
	data.str = 17;
	root = insertNode(root, data);

	data.key = 8;
	data.str = 8;
	root = insertNode(root, data);

	//root = removeNode(root, 5);	
	//root = removeNode(root, 15);
	//root = removeNode(root, 17);
	//root = removeNode(root, 8);

	data.key = 9;
	data.str = 9;
	root = insertNode(root, data);

	data.key = 90;
	data.str = 90;
	root = insertNode(root, data);

	data.key = 99;
	data.str = 99;
	root = insertNode(root, data);

	data.key = 7;
	data.str = 7;
	root = insertNode(root, data);

	data.key = 6;
	data.str = 6;
	root = insertNode(root, data);

	data.key = 80;
	data.str = 80;
	root = insertNode(root, data);

	data.key = 20;
	data.str = 20;
	root = insertNode(root, data);

	data.key = 50;
	data.str = 50;
	root = insertNode(root, data);

	data.key = 85;
	data.str = 85;
	root = insertNode(root, data);

	data.key = 4;
	data.str = 4;
	root = insertNode(root, data);

	data.key = 10;
	data.str = 10;
	root = insertNode(root, data);


	data.key = 81;
	data.str = 81;
	root = insertNode(root, data);

	data.key = 54;
	data.str = 54;
	root = insertNode(root, data);

	data.key = 23;
	data.str = 23;
	root = insertNode(root, data);

	data.key = 576;
	data.str = 576;
	root = insertNode(root, data);

	
	root = removeNode(root, 23);
	root = removeNode(root, 5);
	root = removeNode(root, 4);
	root = removeNode(root, 80);
	root = removeNode(root, 50);
	root = removeNode(root, 576);
	root = removeNode(root, 20);
	root = removeNode(root, 90);
	root = removeNode(root, 6);
	root = removeNode(root, 85);
	root = removeNode(root, 81);
	root = removeNode(root, 7);

	root = removeNode(root, 15);
	root = removeNode(root, 17);
	root = removeNode(root, 9);	

	root = removeNode(root, 54);
	root = removeNode(root, 10);
	root = removeNode(root, 8);
	root = removeNode(root, 99);
	return 0;
}

