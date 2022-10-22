#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>
#define MAX 100

typedef struct node
{
    int id;             // ID of user
    int numfren;        // number of friends of user
    char name[MAX];     // name of user
    int *friends;       // friends of user as an array
    struct node *right; // user to the right
    struct node *left;  // user to the left
} node;

struct node *retUser(char str[MAX])
{
    char name[MAX];
    char ID[MAX];
    char strfriends[MAX];

    // copied ID
    char *token = strtok(str, ",");
    strcpy(ID, token);

    // copied Name
    token = strtok(NULL, ",");
    strcpy(name, token);

    // copied friends' ID
    token = strtok(NULL, ",");
    strcpy(strfriends, token);
    // printf("%s\n",(strfriends));

    // creating user nodes
    int id = atoi(ID);
    struct node *user = malloc(sizeof(struct node));
    user->id = id;
    user->friends = (int *)malloc(MAX * sizeof(int));
    strcpy(user->name, name);

    // adding user's friend's IDs
    token = strtok(strfriends, "|");
    int i = 0;
    while (token != NULL)
    {

        int temp = atoi(token);
        user->friends[i] = temp;
        i++;
        token = strtok(NULL, "|");
    }
    user->numfren = i;
    if (i == 0)
    {
        user->friends[i] = -1;
    }

    return user;
}

// search for user with id=key
struct node *search(int key, struct node *users)
{
    node *p = users;
    
    while (p != NULL)
    {
        if (p->id == key)   //checking if the node is the key
        {
            return p;
        }
        else if (key > p->id)     //traversing to the right
        {
            p = p->right;
        }
        else                      //traversing to the left
        {
            p = p->left;                  
        }
    }
    return NULL;
}

// see document for explanattion
struct node *refineUser(struct node *user, struct node *users)
{

    user->left = NULL;
    user->right = NULL;
    
    int num = 0;
    
    while (num != 1){
        if (search(user->id, users) != NULL){
            (user->id)++;
        }
        else{
            num = 1;
        }
     }
     
    int count = 0;
    
    while (count != user->numfren){
    
        node *temp = search(user->friends[count], users);
        
        if (temp != NULL){
            count++;
            temp->friends[temp->numfren] = user->id;
            (temp->numfren)++;
        }
        
        else{
        
            user->friends[user->numfren] = 0;

            for(int x = count; x < user->numfren; x++){
                user->friends[x] = user->friends[x + 1];
            }
            
            (user->numfren)--;
        }
    }
    
    if (count == 0){
        user->friends[count] = -1;
    }

    return user;
}

// insert user with id
struct node *insertUser(struct node *root, int id, struct node *user)
{
    node *p = root;
    
    if (root == NULL){
        root = user;
    }
    
    else{
    
        while (1){
        
            if (id >= p->id){

                if (p->right == NULL){
                    p->right = user;
                    break;
                }
                
                else{
                    p = p->right;
                }
            }
            
            else if (id < p->id){
            
                if (p->left == NULL){
                    p->left = user;
                    break;
                }
                
                else{
                    p = p->left;
                }
            }
        }
    }
    
    return root;
}

// prints friends list
void friends(int id, struct node *users)
{
    node *temp = search(id, users);
    
    if (temp != NULL){
    
        if (temp->numfren == 0){
            printf("%d\n", -1);
        }
        
        else{
        
            for (int x = 0; x < temp->numfren; x++)
            {
                printf("%d\n", temp->friends[x]);
            }
        }
    }
    
    else{
        printf("%d\n", -1);
    }
}

//find child node with minimum value (inorder successor) - helper to delete node
 struct node *minValueNode(struct node *node) 
 {
   struct node* temp=node->right;
   
   while(temp -> left!=NULL)
   {
    temp = temp->left;
   }
   
   return temp;
 }

// deletes itself from its friend's nodes
struct node *deleteFriends(int key, struct node *users)
{
    node *p = search(key, users);
    if (p == NULL)
        return users;

    for (int x = 0; x < p->numfren; x++)
    {
        node *q = search(p->friends[x], users);
        
        for (int y = 0; y < q->numfren; y++){
        
            if (q->friends[y] == key){
            
                q->friends[q->numfren] = 0;
                
                for (int z = y; z < q->numfren; z++){
                
                    q->friends[z] = q->friends[z + 1];
                }
                
                --q->numfren;
                
                break;
            }
            
            else{
                y++;
            }
        }
    }

    return users;
}

// Deleting a node
struct node *deleteNode(struct node *root, int key)
{

    if (root == NULL){
        printf("empty!!");
    }
    
    else
    {
        if (key > root->id){
            root->right = deleteNode(root->right, key);
        }
        
        else if (key < root->id){
            root->left = deleteNode(root->left, key);
        }
        
        
        else if (root->left == NULL){
            node *q = root->right;
            free(root);
            return q;
        }
        
        else if (root->right == NULL){
            node *q = root->left;
            free(root);
            return q;
        }
        
        
        else{
            node *q=minValueNode(root);
            root->id = q->id;
            strcpy(root->name, q->name);
            root->right = deleteNode(root->right, q->id);
        }
    }

    return root;
}

// Print USER's IDs in ascending order
void printInOrder(node *myusers)
{
    if (myusers !=NULL){
        printInOrder(myusers->left);
        
        printf("%d %s\n", myusers->id, myusers->name);
        
        printInOrder(myusers->right);
    }
}


int main(int argc, char **argv)
{
    node *users = NULL;
    while (1)
    {

        int opt, id;
        fflush(stdin);
        scanf("%d", &opt);
        char str[MAX];
        switch (opt)
        {
        case 1:

            scanf("%s", str);
            struct node *tbins = retUser(str);
            tbins = refineUser(tbins, users);

            users = insertUser(users, tbins->id, tbins);
            break;

        case 2:

            scanf("%d", &id);
            deleteFriends(id, users);
            users = deleteNode(users, id);

            break;

        case 3:

            scanf("%d", &id);
            node *result = search(id, users);
            if (result == NULL)
                printf("USER NOT IN TREE.\n");
            else
            {
                printf("%d\n", result->id);
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
