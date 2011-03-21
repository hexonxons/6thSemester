#include <stdio.h>
#include <stdlib.h>
#include "binarySearchTree.h"

//#define treeNode struct treeNode
//#define nodeData struct nodeData


int main(int argc, char **argv)
{
	struct treeNode *tree;
	struct nodeData data;
	struct treeNode *node;
	
	//tree = (struct nodeData *) calloc(sizeof(struct treeNode *), 1);
	//memAllocErrorCheck(__FILE__, __LINE__, tree);
	//data = malloc(sizeof(nodeData));
	
	data.key = 5;
	data.str = 5;
	tree = initTree(data);

	data.key = 4;
	data.str = 4;
	insertNode(tree, data);
	
	data.key = 10;
	data.str = 10;
	insertNode(tree, data);

	data.key = 8;
	data.str = 8;
	insertNode(tree, data);
	
	data.key = 9;
	data.str = 9;
	insertNode(tree, data);
	
	data.key = 7;
	data.str = 7;
	insertNode(tree, data);
	
	data.key = 6;
	data.str = 6;
	insertNode(tree, data);
	
	data.key = 100;
	data.str = 100;
	insertNode(tree, data);
	
	data.key = 20;
	data.str = 20;
	insertNode(tree, data);
	
	data.key = 15;
	data.str = 15;
	insertNode(tree, data);
	
	data.key = 17;
	data.str = 17;
	insertNode(tree, data);
	
	data.key = 50;
	data.str = 50;
	insertNode(tree, data);
	
	data.key = 110;
	data.str = 110;
	insertNode(tree, data);
	
	data.key = 105;
	data.str = 105;
	insertNode(tree, data);

	data.key = 200;
	data.str = 200;
	insertNode(tree, data);
	
	data.key = 300;
	data.str = 300;
	insertNode(tree, data);


	node = findNode(tree, 20);
	removeNode(tree, 5);
	removeNode(tree, 4);
	removeNode(tree, 10);
	removeNode(tree, 8);
	removeNode(tree, 7);
	removeNode(tree, 6);
	removeNode(tree, 9);	
	removeNode(tree, 100);
	removeNode(tree, 20);
	removeNode(tree, 15);
	removeNode(tree, 17);
	removeNode(tree, 50);
	removeNode(tree, 110);
	removeNode(tree, 105);
	removeNode(tree, 200);
	if (findNode(tree, 300)->left != NULL && findNode(tree, 300)->right != NULL)
	{
		fprintf(stderr,"Error in 300\n");
	}
	removeNode(tree, 300);
	return 0;
}

