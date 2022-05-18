#include "StudentTextEditor.h"
#include "Undo.h"
#include <string>
#include <vector>

TextEditor* createTextEditor(Undo* un)
{
	return new StudentTextEditor(un);
}

StudentTextEditor::StudentTextEditor(Undo* undo)
 : TextEditor(undo) {
	cur_row = 0;
	cur_col = 0;
	linescont.push_back("");
	rowNode = linescont.begin();
}

StudentTextEditor::~StudentTextEditor()
{
	linescont.clear();
}

bool StudentTextEditor::load(std::string file) {
	std::string s;
	std::ifstream loadfile(file);
	if (!loadfile) {
		return false;
	}
	int i = 0;
	reset();
	while (getline(loadfile, s)) {
		std::stringstream ss(s);
		std::getline(ss, s, '\r');
		linescont.push_back(s);
		i++;
	}
	rowNode = linescont.begin();
	return true;
}

bool StudentTextEditor::save(std::string file) {
	std::ofstream savefile(file);
	if (!savefile) {
		return false;
	}
	int n = linescont.size();
	std::list<std::string>::iterator it = linescont.begin();
	for (int i = 0; i < n; i++) {
		savefile << *it + '\n';
		it++;
	}
	return true;
}

void StudentTextEditor::reset() {
	linescont.clear();
	getUndo()->clear();
	cur_row = 0;
	cur_col = 0;
}

void StudentTextEditor::move(Dir dir) {
	int n = linescont.size() - 1;
	std::list<std::string>::iterator iter = rowNode;
	switch (dir)
	{
	case TextEditor::UP:
		if (cur_row > 0) {
			int rowabove = (*(--iter)).length();
			if (cur_col > rowabove) {
				cur_col = rowabove;
			}
			cur_row--;
			rowNode--;
		}
		break;
	case TextEditor::DOWN:
		if (cur_row < n) {
			int rowbelow = (*(++iter)).length();
			if (cur_col > rowbelow) {
				cur_col = rowbelow;
			}
			cur_row++;
			rowNode++;
		}
		break;
	case TextEditor::LEFT:
		if (cur_col == 0) {
			if (cur_row > 0) {
				cur_row--;
				rowNode--;
				cur_col = (*(--iter)).length();
			}
		}
		else {
			cur_col--;
		}
		break;
	case TextEditor::RIGHT:
		if (iter != linescont.end()) {
			if (cur_col == (*iter).length()) {
				if (cur_row < n) {
					cur_row++;
					cur_col = 0;
					rowNode++;
				}
			}
			else {
				cur_col++;
			}
		}
		break;
	case TextEditor::HOME:
		cur_col = 0;
		break;
	case TextEditor::END:
		cur_col = (*iter).length();
		break;
	default:
		break;
	}
}

void StudentTextEditor::del() {
	delHelp(true);
}

void StudentTextEditor::backspace() {
	if (cur_col > 0) {
		char erasedchar = (*rowNode)[cur_col - 1];
		rowNode->erase(cur_col - 1, 1);
		move(TextEditor::LEFT);
		getUndo()->submit(Undo::Action::DELETE, cur_row, cur_col, erasedchar);
	}
	if (cur_col == 0) {
		if (cur_row > 0) {
			std::list<std::string>::iterator it = rowNode;
			it--;
			cur_col = it->length();
			*it = *it + *rowNode;
			rowNode = linescont.erase(rowNode);
			rowNode--;
			cur_row--;
			getUndo()->submit(Undo::Action::JOIN, cur_row, cur_col);
		}
	}
}

void StudentTextEditor::insert(char ch) {
	insertHelp(ch, true);
}

void StudentTextEditor::enter() {
	enterHelp(true);
}

void StudentTextEditor::getPos(int& row, int& col) const {
	row = cur_row;
	col = cur_col;
}

int StudentTextEditor::getLines(int startRow, int numRows, std::vector<std::string>& lines) const {
	int n = linescont.size();
	if (startRow < 0 || numRows < 0 || startRow > n) {
		return -1;
	}
	lines.clear();
	int i = 0;
	std::list<std::string>::const_iterator it = rowNode;
	int dist = startRow - cur_row;
	if (dist > 0) {
		for (int j = 0; j < dist; j++) {
			it++;
		}
	}
	else if (dist < 0) {
		for (int j = 0; j < -1 * dist; j++) {
			it--;
		}
	}
	for (; it != linescont.end() && i < numRows;) {
		if (it != linescont.end()) {
			lines.push_back((*it));
		}
		i++;
		it++;
	}
	return lines.size();
}

void StudentTextEditor::undo() {
	int row;
	int col;
	std::string text;
	int count;
	Undo::Action action = getUndo()->get(row, col, count, text);
	if (action == Undo::Action::INSERT) { 
		int dist = row - cur_row;
		if (dist > 0) {
			for (int i = 0; i < dist; i++) {
				cur_row++;
				rowNode++;
			}
		}
		else if (dist < 0) {
			for (int i = 0; i < dist * -1; i++) {
				cur_row--;
				rowNode--;
			}
		}
		cur_col = col;
		int n = text.length();
		for (int i = 0; i < n; i++) { //insert func w/o submit method (to prevent undo of undo)
			insertHelp(text[i], false);
		}
		cur_col -= n;
	}
	else if (action == Undo::Action::DELETE) {
		int dist = row - cur_row;
		if (dist > 0) {
			for (int i = 0; i < dist; i++) {
				cur_row++;
				rowNode++;
			}
		}
		else if (dist < 0) {
			for (int i = 0; i < dist * -1; i++) {
				cur_row--;
				rowNode--;
			}
		}
		cur_col = col;
		for (int i = 0; i < count; i++) { //del func w/o submit method (to prevent undo of undo)
			delHelp(false);
		}
	}
	else if (action == Undo::Action::SPLIT) {
		int dist = row - cur_row;
		if (dist > 0) {
			for (int i = 0; i < dist; i++) {
				cur_row++;
				rowNode++;
			}
		}
		else if (dist < 0) {
			for (int i = 0; i < dist * -1; i++) {
				cur_row--;
				rowNode--;
			}
		}
		cur_col = col; //enter func w/o submit method (to prevent undo of undo)
		enterHelp(false);
		move(TextEditor::Dir::LEFT);
	}
	else if (action == Undo::Action::JOIN) {
		int dist = row - cur_row;
		if (dist > 0) {
			for (int i = 0; i < dist; i++) {
				cur_row++;
				rowNode++;
			}
		}
		else if (dist < 0) {
			for (int i = 0; i < dist * -1; i++) {
				cur_row--;
				rowNode--;
			}
		}
		cur_col = col; //del func w/o submit method (to prevent undo of undo)
		delHelp(false);
	}
	else {
		return;
	}
}

void StudentTextEditor::insertHelp(char ch, bool submit) {
	if (ch == '\t') {
		std::list<std::string>::iterator it = rowNode;
		std::string ph;
		if (it == linescont.end()) {
			ph = "";
			linescont.push_back("");
			rowNode = linescont.begin();
		}
		else {
			ph = *it;
		}
		ph.insert(cur_col, 4, ' ');
		*rowNode = ph;
		move(TextEditor::RIGHT);
		move(TextEditor::RIGHT);
		move(TextEditor::RIGHT);
		move(TextEditor::RIGHT);
		if (submit) {
			getUndo()->submit(Undo::Action::INSERT, cur_row, cur_col, ' ');
		}
		return;
	}

	std::list<std::string>::iterator it = rowNode;
	std::string ph;
	if (it == linescont.end()) {
		ph = "";
		linescont.push_back("");
		rowNode = linescont.begin();
	}
	else {
		ph = *it;
	}
	ph.insert(cur_col, 1, ch);
	*rowNode = ph;
	move(TextEditor::RIGHT);
	if (submit) {
		getUndo()->submit(Undo::Action::INSERT, cur_row, cur_col, ' ');
	}
}

void StudentTextEditor::delHelp(bool submit) {
	char curchar = (*rowNode)[cur_col];
	if (curchar) {
		rowNode->erase(cur_col, 1);
		if (submit) {
			getUndo()->submit(Undo::Action::DELETE, cur_row, cur_col, curchar);
		}
	}
	else {
		std::list<std::string>::iterator it = rowNode;
		it++;
		if (it != linescont.end()) {
			*rowNode += *it;
			linescont.erase(it);
			if (submit) {
				getUndo()->submit(Undo::Action::JOIN, cur_row, cur_col);
			}
		}
	}
}

void StudentTextEditor::enterHelp(bool submit) {
	std::list<std::string>::iterator it = rowNode;
	if (rowNode != linescont.end()) {
		std::string substring = it->substr(cur_col);
		*it = it->substr(0, it->length() - substring.length());
		linescont.insert(++it, substring);
		rowNode++;
		if (submit) {
			getUndo()->submit(Undo::Action::SPLIT, cur_row, cur_col);
		}
		cur_row++;
		cur_col = 0;
	}
	else {
		linescont.push_back("");
		rowNode++;
		if (submit) {
			getUndo()->submit(Undo::Action::SPLIT, cur_row, cur_col);
		}
		cur_row++;
		cur_col = 0;
	}
}