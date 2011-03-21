#define key_t int
#define data_t int

// ��������� ������ � ������
struct nodeData
{
	key_t key; 
	data_t str;
};

// ��������� ���� ������
struct treeNode
{
	struct treeNode *left;
	struct treeNode *right;
	struct treeNode *parent;
	struct nodeData *data;
};

// ������� �������� ������
struct treeNode *initTree(struct nodeData elem);
// ������� �������� ����
struct treeNode *makeNode();
// ������� ������� �������� � ������
struct treeNode *insertNode(struct treeNode *tree, struct nodeData elem);
// ������� �������� �������� �� ������ �� �����
struct treeNode *removeNode(struct treeNode *tree, key_t key);
// ������� ������ ������ � ������ �� �����
struct treeNode *findNode(struct treeNode *tree, key_t key);
// ������� ��������� ������
// ���������� 1, ���� left > right
//			  0, ���� �����
//			 -1, ���� left < right
int keyCompare(key_t left, key_t right);
// ����� ��� ������ ������
void memAllocErrorCheck(const char *file, const unsigned int line, const void *obj);

// �������������� ������� ��� AVL-������
// ���������� ������ ������ � �������� � node
int treeDepth(struct treeNode *node);
// ������� ��������
struct treeNode *singleLeftRotate(struct treeNode *node);
struct treeNode *singleRightRotate(struct treeNode *node);
struct treeNode *doubleLeftRotate(struct treeNode *node);
struct treeNode *doubleRightRotate(struct treeNode *node);