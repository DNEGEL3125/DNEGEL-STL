#pragma once
#include<vector>
#include <stack>
#include<string>
#include<iostream>
#include <queue>
using std::vector;
using std::stack;
using std::to_string;
using std::runtime_error;
using std::queue;

template<typename T>
class CartesianTree {
	struct Node {
		T value;
		size_t index;
		Node* left = nullptr;
		Node* right = nullptr;
		Node() = default;
		Node(T value, size_t index) :value(value), index(index) {}
	};
	Node* root = nullptr;

public:
	CartesianTree(const vector<T>& arr) {
		stack<Node*, vector<Node*>> s;
		for (size_t i = 0; i < arr.size(); ++i) {
			Node* node = new Node(arr[i], i);
			while (!s.empty() && s.top()->value > node->value) {
				node->left = s.top();
				s.pop();
			}
			if (s.empty()) {
				root = node;
			}
			else {
				s.top()->right = node;
			}
			s.emplace(node);
		}
	}

	~CartesianTree() {
		stack<Node*, vector<Node*>> s;
		s.emplace(root);
		while (!s.empty()) {
			Node* node = s.top();
			s.pop();
			if (node->left)
				s.emplace(node->left);
			if (node->right)
				s.emplace(node->right);
			delete node;
			node = nullptr;
		}
	}

	Node& find_lca(size_t left, size_t right) {
		if (not root)
			throw runtime_error("root is null");
		else if (left > right) {
			throw runtime_error("range is illegal");
		}
		Node* node = root;

		while (left > node->index || right < node->index) {
			if (node->index > left) {
				node = node->left;
			}
			else {
				node = node->right;
			}
		}
		if (node == nullptr) {
			throw runtime_error("the range [" + to_string(left) + ", " + to_string(right) + "] isn't in the tree");
		}
		return *node;
	}

	void print() {
		queue<Node*> q;
		q.push(root);
		while (!q.empty()) {
			if (q.front() == nullptr) {
				q.pop();
				continue;
			}
			std::cout << q.front()->value << "\t";
			q.push(q.front()->left);
			q.push(q.front()->right);
			q.pop();
		}
		std::cout << std::endl;
	}
};