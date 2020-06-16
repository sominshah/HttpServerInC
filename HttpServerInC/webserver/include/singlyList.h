typedef struct Node_Singly
{
	struct Node_Singly * next;
	void *data;
}SinglyNode;
typedef struct LinkedList_xyz
{
 SinglyNode * start;
 SinglyNode *end;
int size;
void (*add) (struct LinkedList_xyz *,void *);
int (*getSize)(struct LinkedList_xyz *);
void *  (*get) (struct LinkedList_xyz *,int);
void * (*delete) (struct LinkedList_xyz *,int);
void (*insert)(struct LinkedList_xyz*,int,void *); 
void (*insertAtTop) (struct LinkedList_xyz *,void *);
void (*insertAtBottom) (struct LinkedList_xyz *,void *);

}LinkedList;
void insert_At_Top_SinglyList(LinkedList *this,void * data);
void insert_At_Bottom_SinglyList(LinkedList *this,void * data);
void add_SinglyList(LinkedList *this,void * data);
void * get_SinglyList(LinkedList * this,int index);
int getSize_SinglyList(LinkedList *this);
void * delete_SinglyList(LinkedList * this,int index);
void insert_SinglyList(LinkedList *this,int index,void *data);
void createSinglyList(LinkedList * list);
void deleteSinglyList(LinkedList * list);