#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

int dupe = 0;
int miss = 0;

typedef struct Node {
	struct Node* left, * right;
	int height;
	int key;
} Node;

int hieght(Node* now) {
	if (now == NULL)
		return 0;
	else
		return now->height;
}

int maximum(int x, int y) {
	return (x > y) ? x : y;
}

int difference(Node* now) {
	if (now == NULL)
		return 0;
	else
		return hieght(now->right) - hieght(now->left);
}

Node* small_left_rotation(Node* now) {
	Node* now_right = now->right;
	Node* next_left = now_right->left;

	now->right = next_left;

	now_right->left = now;
	now_right->height = maximum(hieght(now_right->left), hieght(now_right->right)) + 1;

	now->height = maximum(hieght(now->left), hieght(now->right)) + 1;

	return now_right;
}

Node* small_right_rotation(Node* now) {
	//printf("\n\n%d\n", now->key);
	Node* now_left = now->left;
	Node* next_right = now_left->right;

	now->left = next_right;

	now_left->right = now;
	now_left->height = maximum(hieght(now_left->left), hieght(now_left->right)) + 1;

	now->height = maximum(hieght(now->left), hieght(now->right)) + 1;

	return now_left;
}

Node* big_left_rotation(Node* key) {
	key->right = small_right_rotation(key->right);
	return small_left_rotation(key);
}

Node* big_right_rotation(Node* key) {
	key->left = small_left_rotation(key->left);
	return small_right_rotation(key);
}

Node* add(Node* now, int value) {
	if (now == NULL) {
		Node* new_node = (Node*)malloc(sizeof(Node));

		new_node->left = NULL;
		new_node->right = NULL;
		new_node->key = value;
		new_node->height = 1;

		return new_node;
	}

	//printf("\n\n%d\n", now);

	if (now->key == value) {
		dupe = 1;
		printf("dupe\n");
		return now;
	}

	if (now->key < value)
		now->right = add(now->right, value);

	else
		now->left = add(now->left, value);

	now->height = maximum(hieght(now->left), hieght(now->right)) + 1;

	int diff = difference(now);

	if (diff > 1) {
		//printf("OK  %d\n", diff);
		if (difference(now->right) >= 0)
			return small_left_rotation(now);
		else
			return big_left_rotation(now);
	}

	if (diff < -1) {
		//printf("OK  %d\n", diff);
		if (difference(now->left) <= 0)
			return small_right_rotation(now);
		else
			return big_right_rotation(now);
	}

	return now;
}

Node* removve(Node* now, int value) {
	if (now == NULL) {
		miss = 1;
		printf("miss\n");
		return now;
	}

	if (now->key < value)
		now->right = removve(now->right, value);

	else {
		if (now->key > value)
			now->left = removve(now->left, value);

		else { // нашли искомый ключ
			Node* ptr, *left = now->left, *right = now->right;

			if (left == NULL && right == NULL) { // нет детей
				ptr = now;
				now = NULL;
				free(ptr);
				return now;
			}

			if (left != NULL && right == NULL) { // есть левый ребенок
				*now = *(left);
				free(left);
			}

			else {
				if (left == NULL && right != NULL) { // есть правый ребенок
					*now = *(right);
					free(right);
				}

				else { // есть оба ребенка
					ptr = left;
					while (ptr->right != NULL)
						ptr = ptr->right;

					now->key = ptr->key;
					now->left = removve(now->left, now->key);
				}
			}
		}
	}

	now->height = maximum(hieght(now->left), hieght(now->right)) + 1;

	int diff = difference(now);

	if (diff > 1) {
		if (difference(now->right) >= 0)
			return small_left_rotation(now);
		else
			return big_left_rotation(now);
	}

	if (diff < -1) {
		if (difference(now->left) <= 0)
			return small_right_rotation(now);
		else
			return big_right_rotation(now);
	}

	return now;
}

void lower(Node* now, int value) {
	int key, last_value = 1000000009;
	if (now == NULL) {
		printf("###\n");
		return;
	}

	while (now != NULL) {
		key = now->key;
		if (key == value) {
			printf("%d\n", value);
			return;
		}

		if (key > value) {
			//if (now->key < last_value)
			last_value = key;
			now = now->left;
		}

		else
			now = now->right;
	}

	if (last_value != 1000000009)
		printf("%d\n", last_value);
	else
		printf("###\n");
}

int main() {
	Node* root = NULL;
	int m, value;
	char comand[8];

	scanf("%d", &m);
	for (int i = 0; i < m; i++) {
		scanf("%s", comand);
		scanf("%d", &value);

		//printf("COMAND %d: ", i + 1);
		//if (i == 7 || i == 8) {
			//printf("\n\n%d    %d    %d\n", root->key, root->left, root->right);
		//}


		if (comand[0] == 'a') {
			root = add(root, value);
			if (dupe == 1)
				dupe = 0;
			else
				printf("added\n");

			//printf("\n\n%d %d\n", root->left, root->right);
		}

		if (comand[0] == 'r') {
			root = removve(root, value);

			if (miss == 1)
				miss = 0;
			else
				printf("removed\n");
		}

		if (comand[0] == 'l')
			lower(root, value);

		//if (i == 9)
			//printf("\n\n%d %d %d %d\n", root->key, root->right->key, root->right->left->key, root->right->right->key);
	}

	return 0;
}
