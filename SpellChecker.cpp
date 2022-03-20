
#include <iostream>
#include <string>
#include <stack>
#include <vector>
#include <fstream>
#include <algorithm>
#include <chrono>

using namespace std;

// define caracter size
#define CHAR_SIZE 26

//calculate minimum between two values
#define MIN(x,y) ((x) < (y) ? (x) : (y))

class Trie {
public:
	bool isLeaf;
	Trie *character[CHAR_SIZE];
	char c;
	string word;

	//contructor
	Trie() {
		this->isLeaf = false;
		for (int i = 0; i < CHAR_SIZE; i++) {
			this->character[i] = nullptr;
		}
	}
	void insert(string);
	bool search(string);
};


// iterative function to insert a key in the Trie
void Trie::insert(string key) {
	// start from the root node
	Trie *curr = this;
	for (int i = 0; i < key.length(); i++) {
		if (curr->character[key[i] - 0x41] == nullptr) {
			curr->character[key[i] - 0x41] = new Trie();
		}
		//go to the next node
		curr->character[key[i] - 0x41]->c = key[i];
		curr = curr->character[key[i] - 0x41];
	}
	// marke the current node as a lear
	curr->isLeaf = true;
}

bool Trie::search(string key) {
	// return fale is Trie is empty
	if (this == nullptr) {
		return false;
	}
	Trie *curr = this;
	for (int i = 0; i < key.length(); i++) {
		// go to next node
		curr = curr->character[key[i] - 0x41];
		if (curr == nullptr) {
			return false;
		}
	}
	// return true if current node is a leaf
	return curr->isLeaf;
}


int LevenshteinDistance(string s1, string s2) {
	int i, j, l1, l2, t, track;
	int dist[50][50];
	l1 = s1.length();
	l2 = s2.length();
	for (i = 0; i <= l1; i++) {
		dist[0][i] = i;
	}
	for (j = 0; j <= l2; j++) {
		dist[j][0] = j;
	}
	for (j = 1; j <= l1; j++) {
		for (i = 1; i <= l2; i++) {
			if (s1[i - 1] == s2[j - 1]) {
				track = 0;
			}
			else {
				track = 1;
			}
			t = MIN((dist[i - 1][j] + 1), (dist[i][j - 1] + 1));
			dist[i][j] = MIN(t, (dist[i - 1][j - 1] + track));
		}
	}
	return dist[l2][l1];
}

bool inRange(unsigned low, unsigned high, unsigned x)
{
	return (low <= x && x <= high);
}


/* Iterative function for inorder tree traversal */
string inOrder(struct Trie *root, string key) {
	stack<Trie *> stack;
	Trie *curr = root;
	string matchedWord;
	int LevenshteinDis = 0xFF;
	curr->word = "";
	stack.push(curr);
	while (stack.size()) {
		curr = stack.top();
		stack.pop();
		if (curr->word.size() <= key.size() + 1) {
			for (int i = 0; i < CHAR_SIZE; i++) {
				if (curr->character[i] != nullptr) {

					Trie *t = curr->character[i];
					t->word = curr->word;
					t->word.push_back(curr->character[i]->c);

					if (curr->character[i]->isLeaf && (inRange(key.length() - 1, key.length() + 1, t->word.length()))) {
						if (LevenshteinDistance(key, t->word) < LevenshteinDis) {
							LevenshteinDis = LevenshteinDistance(key, t->word);
							matchedWord = t->word;
						}
					}
					stack.push(t);
				}
			}
		}

	}
	return matchedWord;
}

int main() {

	vector<string> Dictonary;
	vector<string> InputFIle;
	string word;


	cout << "Reading Dictionary..." << endl;
	ifstream src("/tmp/dictionary.txt");
	// copy dictionary to vector //
	if (!src) {
		cout << "Ditonary not found" << endl;
		exit(1);
	}

	while (src >> word) {
		//cout << word << endl;  //Just for testing
		Dictonary.push_back(word);
	}

	cout << "Reading input file..." << endl;
	ifstream file("/tmp/input.txt");
	// copy input file to vector //
	if (!file) {
		cout << "Error opening the file" << endl;
		exit(1);
	}
	// extracting words from the file
	while (file >> word) {
		transform(word.begin(), word.end(), word.begin(), ::toupper);
		InputFIle.push_back(word);
	}

	// Initialize a Trie
	Trie *root = new Trie();

	// Insert strings to trie
	for (int i = 0; i < Dictonary.size(); i++) {
		root->insert(Dictonary[i]);
	}

	cout << "Starting Analysis of input file..." << endl;

	auto start = std::chrono::system_clock::now();

	for (auto w = InputFIle.begin(); w != InputFIle.end(); w++) {
		if (root->search(*w)) {
			cout << *w << " : found " << endl;
		}
		else {
			cout << "matched word for "<<*w <<" : "<< inOrder(root, *w) << endl;
		}
	}
	auto end = std::chrono::system_clock::now();
	std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << "ms" << std::endl;

	return 0;
}

