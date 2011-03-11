#define key_t int
#define data_t int

// структура данных в дереве
struct nodeData
{
	key_t key; 
	data_t str;
};

// структура узла дерева
struct treeNode
{
	struct treeNode *left;
	struct treeNode *right;
	struct treeNode *parent;
	struct nodeData *data;
};

// функция создания дерева
struct treeNode *initTree(struct nodeData elem);
// функция создания нода
struct treeNode *makeNode();
// функция вставки элемента в дерево
struct treeNode *insertNode(struct treeNode *tree, struct nodeData elem);
// функция удаления элемента из дерева по ключу
struct treeNode *removeNode(struct treeNode *tree, key_t key);
// функция поиска данных в дереве по ключу
struct treeNode *findNode(struct treeNode *tree, key_t key);
// функция сравнения ключей
// возвращает 1, если left > right
//			  0, если равны
//			 -1, если left < right
int keyCompare(key_t left, key_t right);
// вызов при ошибке памяти
void memAllocErrorCheck(const char *file, const unsigned int line, const void *obj);

// дополнительные функции для AVL-дерева
// вычисление высоты дерева в вершиной в node
int treeDepth(struct treeNode *node);
// функции вращения
struct treeNode *singleLeftRotate(struct treeNode *node);
struct treeNode *singleRightRotate(struct treeNode *node);
struct treeNode *doubleLeftRotate(struct treeNode *node);
struct treeNode *doubleRightRotate(struct treeNode *node);