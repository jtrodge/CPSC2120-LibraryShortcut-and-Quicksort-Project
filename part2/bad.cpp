#include <iostream>
#include <cstdlib>
#include <tuple>
#include <assert.h>
using namespace std;

struct Node {
  int key;
  int size;
  Node *left, *right;
  Node (int k) { key = k; size = 1; left = right = NULL; }
};

void update_size(Node *n)
{
  n->size = 1;
  if (n->left != NULL) n->size += n->left->size;
  if (n->right != NULL) n->size += n->right->size;
}

// Insert new value v so it becomes the element at rank r
Node *insert(Node *root, int r, int v)
{
  if (root == NULL) return new Node(v);
  assert (r >= 0 && r <= root->size);
  int root_rank = root->left == NULL ? 0 : root->left->size;
  if (r <= root_rank) root->left  = insert(root->left,  r, v);
  else                root->right = insert(root->right, r - root_rank - 1, v);
  update_size (root);
  return root;
}

// prints out the inorder traversal of T (i.e., the contents of T in sorted order)
void print_inorder(Node *root)
{
  if (root == NULL) return;
  print_inorder(root->left);
  cout << root->key << "\n";
  print_inorder(root->right);
}


// Split tree on rank r into tree L (containing keys of ranks < r) and tree R
pair<Node *, Node *> split(Node *root, int r)
{
  pair <Node *, Node *> result = make_pair (root, root);
  if (root == NULL) return result;
  assert (r >= 0 && r <= root->size);
  int root_rank = root->left == NULL ? 0 : root->left->size;
  if (r <= root_rank) tie (result.first, root->left)   = split (root->left, r);
  else tie (root->right, result.second) = split (root->right, r - root_rank - 1);
  update_size (root);
  return result;
}

// Insert new value v so it becomes the element at rank r
Node *insert_keep_balanced(Node *root, int r, int v)
{
  if (root == NULL) return new Node(v);
  assert (r >= 0 && r <= root->size);
  int root_rank = root->left == NULL ? 0 : root->left->size;
  if (rand() % (1 + root->size) == 0) {
    Node *new_root = new Node(v);
    tie (new_root->left, new_root->right) = split (root, r);
    update_size (new_root);
    return new_root;
  }
  if (r <= root_rank) root->left  = insert_keep_balanced(root->left,  r, v);
  else root->right = insert_keep_balanced(root->right, r - root_rank - 1, v);
  update_size (root);
  return root;
}
int get_rand(int N)
{
  return 123456789 % N;
}

int main(void) {
  //Quick sort - Takes a number, any value that is bigger than that number
  //goes right, any value that has a smaller value goes left
  Node * T = NULL;
  int N = 0;
  cin >> N;
  cout << N << endl;
  //Inserting elements into BST
  for (int i = 0; i < N; i++) {
    //get_rand(N - i) - correct index
    T = insert_keep_balanced(T, get_rand(i+1), N - i);
  }
  print_inorder(T);

  return 0;
}
