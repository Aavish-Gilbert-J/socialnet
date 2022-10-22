#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include<stdbool.h>
#define MAX 100

typedef struct node 
{
    int id; //ID of user
    int numfren; //number of friends of user
    char name[MAX]; //name of user
    int* friends; //friends of user as an array
    struct node* right;  //user to the right
    struct node* left; //user to the left
} node;

struct node* retUser(char str[MAX])                                                               //prewritten function
{
    char name[MAX];
    char ID[MAX];
    char strfriends[MAX];

    //copied ID
    char*token=strtok(str,",");
    strcpy(ID,token);

    //copied Name
    token=strtok(NULL,",");
    strcpy(name,token);

    //copied friends' ID
    token=strtok(NULL,",");
    strcpy(strfriends,token);
    //printf("%s\n",(strfriends));

    //creating user nodes
    int id=atoi(ID);
    struct node *user = malloc(sizeof(struct node));
    user->id=id;
    user->friends=(int*)malloc(MAX * sizeof(int));
    strcpy(user->name,name);


    //adding user's friend's IDs
    token = strtok(strfriends,"|");  
    int i=0;
    while( token != NULL ) 
    {

        int temp=atoi(token);
        user->friends[i]=temp;
        i++;
        token = strtok(NULL,"|");
    }
    user->numfren=i;
    if(i==0){user->friends[i]=-1;}
    return user;
}


//search for user with id=key
struct node* search(int key, struct node *users)                                                  //might work- completed
{
    if (users == NULL)
        return NULL;
 
    /* first recur on left child */
    search(key, users->left);
 
    /* check if the id matches with the key */
    if(users->id==key){
        return users;
    }
 
    /* now recur on right child */
    search(key, users->right);
}
 

int check_user(struct node *root, int id)                                                      //self defined works
{
    struct node *curr = root;
    while(curr != NULL){
        if(curr->id == id) return 1;
        if(curr->id > id) curr = curr->right;
        else curr = curr->left;
    }
    if(curr == NULL) return 0;
}


void add_bidirectional_friend(int user_id, struct node *node)                                   //self defined works
{ 
    node->friends[node->numfren++] = user_id;
}


//see document for explanattion
struct node*refineUser(struct node*user, struct node *users)                                     //works
{
    while(check_user(users, user->id) == 1){
        user->id++;
    }
    if(user->numfren == 0){
        user->friends[user->numfren]=-1;
        user->numfren=0;
        return(user);
    }
    for(int i=0; i<user->numfren; i++){
        if(search(user->friends[i], users) != NULL){
            struct node *Node = search(user->friends[i], users);
            add_bidirectional_friend(user->id, Node);
        }
    }
    return user;
}

//insert user with id
struct node* insertUser(struct node*root,int id,struct node*user)                                //might work- completed
{
   if(root==NULL)
   {
    struct node* temp = (struct node*)malloc(sizeof(struct node));
    temp = user;
    return temp;
   }

   if (id < root->id)
        root->left = insertUser(root->left, id, user);
    else if (id > root->id)
        root->right = insertUser(root->right, id, user);

    printf("inserted");
 
    /* return the (unchanged) node pointer */
    return root;
}

//prints friends list  
void friends(int id, struct node *users)                                                         //might work - completed
{
    if(users!=NULL)
    {
        /* first recur on left child */
        friends(id, users->left);
    
        /* check if the id matches with the key */
        if(users->id == id)
        {
            int i;
            for(i=0; i < users->numfren; i++)
            {
                printf("%d", users->friends[i]);
            }
        }
    
        /* now recur on right child */
        friends(id, users->right);

    }
}

//find child node with minimum value (inorder successor) - helper to delete node
struct node *minValueNode(struct node *node) {                                                   //might work - completed
    struct node *p;
    p=node->right;
    while(p->left!=NULL) p=p->left;
    return p;
}

//deletes itself from its friend's nodes
struct node*deleteFriends(int key, struct node*users)                                            //might work - completed
{
    if (users == NULL)
    return NULL;
 
    /* first recur on left child */
    deleteFriends(key, users->left);
 
    /* check if the id matches with the key */
    int i;
    for(i=0; i < users->numfren; i++){
        if(users->friends[i] == key){
            users->friends[i]=-1;
        }
    }
 
    /* now recur on right child */
    deleteFriends(key, users->right);
}

// Deleting a node
struct node *deleteNode(struct node *root, int key) {                                            //might work- completed
   struct node *p;

    if(root==NULL) return root;

    if(key < root->id) 
    {
        root->left = deleteNode(root->left, key);
    }
    else
    {
        if(key > root->id)
        {
            root->right = deleteNode(root->right, key);
        }
    }

    if(root->right==NULL)
    {
        p=root->left;
        free(root);
        return p;    
    }

    else if(root->left==NULL){
        p=root->right;
        free(root);
        return p;
    }

    else{
        // p=root->right;
        // while(p->left!=NULL) p=p->left;

        p=minValueNode(root);
        root->id=p->id;                                 //try root=p if there is an error to preserve the array info
        root->right=deleteNode(root->right, p->id);
    }

    return root;
    
}

//Print USER's IDs in ascending order
void printInOrder(node* myusers)                                                                 //might work- completed
{
    if (myusers == NULL)
        return;
 
    /* first recur on left child */
    printInOrder(myusers->left);
 
    /* then print the data of node */
    printf("%d ", myusers->id);
 
    /* now recur on right child */
    printInOrder(myusers->right);
}


int main(int argc, char **argv)
{
    node *users=NULL;   
    while(1)
    {

        int opt, id;
        fflush(stdin);
        scanf("%d",&opt);
        char str[MAX];
        switch (opt)
        {
            case 1:
      
                scanf("%s",str);
                struct node*tbins=retUser(str);
                tbins=refineUser(tbins, users);
                users=insertUser(users,tbins->id,tbins);
                break;

            case 2:
           
                scanf("%d", &id);
                deleteFriends(id, users);
                users=deleteNode(users, id);
                break;

            case 3:
        
                scanf("%d", &id);
                node* result=search(id,users);
                if(result==NULL) 
                    printf("USER NOT IN TREE.\n");
                else{
                    printf("%d\n",result->id);
                }
                break;

            case 4:
                scanf("%d", &id);
                friends(id, users);
                break;

            case 5:
                printInOrder(users);
                break;

            case 6:
                exit(0);
                break;

            default: 
                printf("Wrong input! \n");
                break;
        }

    }
    return 0;
}








// friends
// print inorder