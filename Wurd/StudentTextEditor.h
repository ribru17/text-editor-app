#ifndef STUDENTTEXTEDITOR_H_
#define STUDENTTEXTEDITOR_H_

#include "TextEditor.h"
#include <unordered_map>
#include <string>
#include <iostream>
#include <fstream>
#include <utility>
#include <sstream>

class Undo;

class StudentTextEditor : public TextEditor {
public:

	StudentTextEditor(Undo* undo);
	~StudentTextEditor();
	bool load(std::string file);
	bool save(std::string file);
	void reset();
	void move(Dir dir);
	void del();
	void backspace();
	void insert(char ch);
	void enter();
	void getPos(int& row, int& col) const;
	int getLines(int startRow, int numRows, std::vector<std::string>& lines) const;
	void undo();

private:

	//std::unordered_map<int, std::string> linescont;
	std::list<std::string> linescont;
	std::list<std::string>::iterator rowNode;
	int cur_row;
	int cur_col;
	void insertHelp(char ch, bool submit);
	void delHelp(bool submit);
	void enterHelp(bool submit);
};

#endif // STUDENTTEXTEDITOR_H_
