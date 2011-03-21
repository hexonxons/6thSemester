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
void insertNode(struct treeNode *tree, struct nodeData elem);
// функция удаления элемента из дерева по ключу
void removeNode(struct treeNode *tree, key_t key);
// функция поиска данных в дереве по ключу
struct treeNode *findNode(struct treeNode *tree, key_t key);
// функция сравнения ключей
int keyCompare(key_t left, key_t right);
// вызов при ошибке  памяти
void memAllocErrorCheck(const char *file, const unsigned int line, const void *obj); 