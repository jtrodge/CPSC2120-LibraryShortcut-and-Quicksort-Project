#include <iostream>
#include <string>
#include <algorithm>
#include <cstdlib>
#include <tuple>
#include <assert.h>
#include <map>
#include <vector>
#include <set>

using namespace std;

struct Node {
  string key;
  int size;
  Node *left, *right, *parent;
  Node (string k) { key = k; size = 1; left = right = parent = NULL; }
};

void update_size(Node *n)
{
  n->size = 1;
  if (n->left != NULL) {
    n->size += n->left->size;
    n->left->parent = n;
  }
  if (n->right != NULL) {
    n->size += n->right->size;
    n->right->parent = n;
  }
}

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
  int root_rank = root->left == NULL ? 0 : root->left->size;
  if (r <= root_rank) tie (result.first, root->left)   = split (root->left, r);
  else tie (root->right, result.second) = split (root->right, r - root_rank - 1);
  update_size (root);
  return result;
}

// Insert new value v so it becomes the element at rank r
Node *insert_keep_balanced(Node *root, int r, Node *v)
{
  if (root == NULL) return v;
  int root_rank = root->left == NULL ? 0 : root->left->size;
  if (rand() % (1 + root->size) == 0) {
    Node *new_root = v;
    tie (new_root->left, new_root->right) = split (root, r);
    update_size (new_root);
    return new_root;
  }
  if (r <= root_rank) root->left  = insert_keep_balanced(root->left,  r, v);
  else root->right = insert_keep_balanced(root->right, r - root_rank - 1, v);
  update_size (root);
  return root;
}

Node *join(Node *L, Node *R)
{
  if (L == NULL) return R;
  if (R == NULL) return L;
  Node *root = (rand() % (L->size + R->size) < L->size) ? L : R;
  if (root == L) L->right = join(L->right, R);
  else           R->left  = join(L, R->left);
  update_size (root);
  return root;
}

// Remove the rank-r element from a tree (should only be called on a non-empty tree)
Node *remove(Node *root, int r)
{
  assert (root != NULL);
  int root_rank = root->left == NULL ? 0 : root->left->size;
  if (r == root_rank) {
    Node *result = join(root->left, root->right);
    delete root;
    return result;
  }
  if (r < root_rank) root->left  = remove (root->left, r);
  else               root->right = remove (root->right, r - root_rank - 1);
  update_size (root);
  return root;
}
int GetRank(Node *T) {
  int leftSize = T->left ? T->left->size : 0;
  if (T->parent == NULL) { return leftSize; }
  if (T == T->parent->right) { return GetRank(T->parent) + leftSize + 1; }
  else { return GetRank(T->parent) - T->size + leftSize; }
}

//Track all unique names - Sort - Ranked based tree
//Print Inorder, split, insert_keep_balanced, resize
int main() {
  int insert = 0;
  string * x;
  string * y;
  cout << "How many names do you want to insert: ";
  cin >> insert;
  x = new string[insert];
  y = new string[insert];
  for (int i = 0; i < insert; i++) {
    cin >> x[i] >> y[i];
  }
  vector<pair<string, string>> names;
  set<string> a;
  //Store strings into set - Remove duplicates / Sort strings
  for (int i = 0; i < insert; i++) {
    names.push_back(make_pair(x[i], y[i]));
    a.insert(names[i].first);
    a.insert(names[i].second);
  }
  Node *S = NULL;
  int rank = 0;
  map<string, Node *> T; //Map string name to the pointer of the BSTmap
  map<string, int> R;
  int work = 0;
  //Store strings from set into ranked based BST
  for (auto x : a) {
    Node *temp = new Node(x);
    S = insert_keep_balanced(S, rank, temp);
    rank++;
    //Map strings to nodes;
    T[x] = temp;
    //Map string to ints
    R[x] = work;
  }
  for (int i = 0; i < insert; i++) {
    string first = names[i].first;
    string second = names[i].second;
    //Get rank to keep track of ranks/Nodes
    int from = GetRank(T[first]);
    int to = GetRank(T[second]);
    //moving from index i to index j
    if (from - to != 1) {
      //Remove string from the rank from
      S = remove(S, from);
      Node *temp = new Node(first);
      T[first] = temp;
      //Insert string back into the tree
      S = insert_keep_balanced(S, to + 1, temp);
      work = abs(from - to);
      if (to < from) {
        work = work - 1;
      }
      R[first] = R[first] + work;
    }
  }
  cout << endl;
  print_inorder(S);
  cout << endl;
  //print out contents of name and work done
  for (auto x : a) {
    cout << x << " " << R[x] << endl;
  }


  return 0;
}
