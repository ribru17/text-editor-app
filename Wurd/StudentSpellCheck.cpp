#include "StudentSpellCheck.h"
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>

SpellCheck* createSpellCheck()
{
	return new StudentSpellCheck;
}

StudentSpellCheck::~StudentSpellCheck() {
	killTrie(root);
}

bool StudentSpellCheck::load(std::string dictionaryFile) {
	std::ifstream dict(dictionaryFile);
	std::string line;
	if (!dict) {
		return false;
	}
	while (getline(dict, line)) {
		int i = 0;
		Trie* pos = root;
		for (int n = line.length(); i < n; i++) {
			if (isalpha(line[i]) || line[i] == '\'') {
				int index;
				if (isalpha(line[i])) {
					index = tolower(line[i]) - 'a';
				}
				else {
					index = 26;
				}
				if (!pos->children[index]) {
					Trie* newNode = new Trie(tolower(line[i]));
					pos->children[index] = newNode;
					pos = newNode;
				}
				else {
					pos = pos->children[index];
				}
			}
		}
		pos->isEnd = true;
	}
	return true;
}

bool StudentSpellCheck::spellCheck(std::string word, int max_suggestions, std::vector<std::string>& suggestions) {
	int n = word.length();
	Trie* curNode = root;
	bool isFound = true;
	int i = 0;
	std::string str = "";
	std::vector<std::string> phvec;
	bool found = false;
	findSug(word, 0, false, root, phvec, str, found);
	if (found) {
		return true;
	}
	suggestions.clear();
	int smaller = max_suggestions > phvec.size() ? phvec.size() : max_suggestions;
	for (int j = 0; j < smaller; j++) {
		suggestions.push_back(phvec[j]);
	}
	return false; 
}

void StudentSpellCheck::spellCheckLine(const std::string& line, std::vector<SpellCheck::Position>& problems) {
	int start = 0;
	int end;
	int n = line.size();
	problems.clear();
	for (int i = 0; i < n; i++) {
		if (isalpha(line[i]) || line[i] == '\'') {
			if (i == 0 || (!isalpha(line[i - 1]) && line[i - 1] != '\'')) {
				start = i;
			}
			if (i == line.size() - 1) {
				end = i;
				Position newPos;
				newPos.start = start;
				newPos.end = end;
				std::string word = line.substr(start, end - start + 1);
				int n = word.length();
				Trie* curNode = root;
				bool isFound = true;
				int i = 0;
				for (; i < n; i++) {
					if (isalpha(word[i]) && curNode->children[tolower(word[i]) - 'a']) {
						curNode = curNode->children[tolower(word[i]) - 'a'];
						if (i == n - 1 && !curNode->isEnd) {//make sure word doesn't just exist, but is also a real word
							isFound = false;
							break;
						}
						continue;
					}
					if (word[i] == '\'' && curNode->children[26]) {
						curNode = curNode->children[26];
						if (i == n - 1 && !curNode->isEnd) {//make sure word doesn't just exist, but is also a real word
							isFound = false;
							break;
						}
						continue;
					}
					isFound = false;
					break;
				}
				if (!isFound) {
					problems.push_back(newPos);
				}

			}
		}
		else {
			if (i != 0 && (isalpha(line[i - 1]) || line[i - 1] == '\'')) {
				end = i - 1;
				Position newPos;
				newPos.start = start;
				newPos.end = end;
				std::string word = line.substr(start, end - start + 1);
				int n = word.length();
				Trie* curNode = root;
				bool isFound = true;
				int i = 0;
				for (; i < n; i++) {
					if (isalpha(word[i]) && curNode->children[tolower(word[i]) - 'a']) {
						curNode = curNode->children[tolower(word[i]) - 'a'];
						if (i == n - 1 && !curNode->isEnd) {//make sure word doesn't just exist, but is also a real word
							isFound = false;
							break;
						}
						continue;
					}
					if (word[i] == '\'' && curNode->children[26]) {
						curNode = curNode->children[26];
						if (i == n - 1 && !curNode->isEnd) {//make sure word doesn't just exist, but is also a real word
							isFound = false;
							break;
						}
						continue;
					}
					isFound = false;
					break;
				}
				if (!isFound) {
					problems.push_back(newPos);
				}
			}
		}
	}
}

void StudentSpellCheck::killTrie(Trie* root) {
	if (root == nullptr) {
		return;
	}
	int n = root->children.size();
	for (int i = 0; i < n; i++) {
		killTrie(root->children[i]);
	}
	delete root;
}

void StudentSpellCheck::findSug(std::string word, int depth, bool oneoff, Trie* node, std::vector<std::string>& strvec, std::string curstr, bool& found) {
	for (int i = 0; i < 27; i++) {
		if (node->children[i]) {
			if (depth < word.size()) {
				if (node->children[i]->val == tolower(word[depth])) {
					std::string ph = curstr;
					ph.append(1, node->children[i]->val);
					int n = word.length();
					if (node->children[i]->isEnd && depth == n - 1) {
						if (oneoff) {
							strvec.push_back(ph);
						}
						else {
							found = true;
						}
					}
					else {
						findSug(word, depth + 1, oneoff, node->children[i], strvec, ph, found);
					}
				}
				else if (node->children[i]->val != tolower(word[depth]) && !oneoff) {
					std::string ph = curstr;
					ph.append(1, node->children[i]->val);
					int n = word.length();
					if (node->children[i]->isEnd && depth == n - 1) {
						strvec.push_back(ph);
					}
					else {
						findSug(word, depth + 1, true, node->children[i], strvec, ph, found);
					}
				}
			}
		}
	}
}