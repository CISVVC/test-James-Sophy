/**
 * Filename: main.cpp
 * Description: Print lines that chosen word is on and its metadata 
 * Author: James Sophy
 * Email: sophyj@student.vvc.edu
 * Date: 11/29/2019
 **/
/*
The relevent code for this final are: struct Word, function read_file, function process_file_input, function print_index, function print_line, function print_word, and function run_program.
I made a fully functioning menu system for this and future projects using current knowledge of c++.
The biggest issue with menu system is that it is mostly hard coded and only has 2 option types.
Another drawback is that only 1 of each option take can be displayed at a time and if their are multiples of the same option type on the same hierarchy level, they are merged if selectors and discarded if inputs.
This menu system uses a hierarchy to function for example to make a quiz using this system you would want all main menu options on 1 or any other number 0.1+, each quiz question options are on the same level 2 and each question is a new hierarchy level so 3 for the next question and hierarchy 4 for the results.
*/

#include <iostream>

#include <fstream>

#include <string>

#include <vector>

#include <algorithm>

using namespace std;

//keep track of a word and its metadata
struct Word {
   string word;
   vector < int > ch;
   vector < int > lines;
};

//keep track of menu options and their hierarchy
struct UI_Option {
   double display_order;
   int priority;
   string type;
   string name;
   vector < string > attributes;
};

struct UI_Options {
   vector < UI_Option > option;
   vector < double > display_order;
};

struct UI_Configs {
   string type;
   int priority_order;
   vector < string > duplicate_handeling;
};

struct UI_Options_Choices {
   double option_order;
   string name;
   string choice;
};

//keep track of menu
struct UI {
   vector < UI_Configs > configs;
   UI_Options options;
   vector < UI_Options_Choices > options_choices;
};

/*
 Read input from file and output to string
 @param file string of filename
 @param output string for adding file input to
*/
void read_file(string file, string & output) {
   string line;
   ifstream in_file(file);
   //check if file was found
   if (in_file.fail()) {
      cout << "Could not open file: " << file << endl;
      return;
   }
   while (getline(in_file, line)) {
      line += '\n';
      output += line;
   }
}

/*
 Grab words and their metadata from input and keep track of all newlines
 @param input string of input
 @param wordL vector<Word> words and their metadata from file input
 @param lines vector<const char*> newlines of file input
*/
void process_file_input(string & input, vector < Word > & wordL, vector <
   const char * > & lines) {
   string word;
   int ch = 0;
   int line = 1;
   bool stored = true;
   const char * c_ptr = input.c_str();
   lines.push_back(c_ptr);
   //first process input per char
   for (int i = 0; i < input.length(); i++) {
      //make sure char is either alphabetical or '
      if (isalpha( * c_ptr) || * c_ptr == '\'') {
         word += * c_ptr;
      } else {
         if (!word.empty()) {
            stored = false;
            //keep track of chapters
            if (word == "CHAPTER") ch++;
            for (Word & words: wordL) {
               //check if word has been found yet
               if (words.word == word) {
                  for (int & cha: words.ch) {
                     //keep track of which chapters word is found in
                     if (cha != ch && cha >= words.ch[words.ch.size() - 1]) words.ch.push_back(ch);
                  }
                  for (int & ln: words.lines) {
                     if (ln != line) {
                        //keep track of which line word occurs on
                        if (line > ln && ln >= words.lines[words.lines.size() - 1]) words.lines.push_back(line);
                     }
                  }
                  stored = true;
                  break;
               }
            }
            //keep track of word and its metadata
            if (!stored) {
               Word w;
               w.word = word;
               w.ch.push_back(ch);
               w.lines.push_back(line);
               wordL.push_back(w);
            }
         }
         word = "";
      }
      //keep list of pointers to each newline
      if ( * c_ptr == '\n' && i != (input.length() - 1)) {
         lines.push_back(c_ptr + 1);
         line++;
      }
      c_ptr++;
   }
}

/*
 Print out specific word and its metadata
 @param word string of word
 @param wordL vector<Word> words and their metadata from file input
*/
void print_index(string word, vector < Word > & wordL) {
   bool found = false;
   string check_word = "";
   string chosen_word = word;
   //transform word to lowercase for catching as many matches as possible
   transform(chosen_word.begin(), chosen_word.end(), chosen_word.begin(), ::tolower);
   for (Word & words: wordL) {
      check_word = words.word;
      //transform word to lowercase for catching as many matches as possible
      transform(check_word.begin(), check_word.end(), check_word.begin(), ::tolower);
      //make sure chosen word was found from input
      if (check_word == chosen_word) {
         found = true;
         //print out the word
         cout << endl << words.word << ", [";
         //print out the words chapter metadata
         for (int & ch: words.ch) {
            if (ch != words.ch[words.ch.size() - 1]) {
               cout << ch << ", ";
            } else {
               cout << ch << "], (";
            }
         }
         //print out the words line occurence metadata
         for (int & line: words.lines) {
            if (line != words.lines[words.lines.size() - 1]) {
               cout << line << ", ";
            } else {
               cout << line << ")";
            }
         }
         cout << endl << endl;
      }
   }
   //if word wasn't found, let user know
   if (!found) cout << endl << "Word: " << word << ", was not found!" << endl << endl;
}

/*
 Prints the text of the given line number
 @param lines vector<const char*> newlines of file input
 @param line int of line number to print
*/
void print_line(vector <
   const char * > lines, int line) {
   while ( * lines[line] != '\n') {
      cout << * lines[line];
      lines[line]++;
   }
   cout << endl;
}

/*
 Print specific word, its metadata and the lines it occurs on
 @param wordL vector<Word> words and their metadata from file input
 @param lines vector<const char*> newlines of file input
 @param word string of word to process
*/
void print_word(vector < Word > & wordL, vector <
   const char * > & lines, string word) {
   bool found = false;
   string check_word = "";
   string chosen_word = word;
   //transform word to lowercase for catching as many matches as possible
   transform(chosen_word.begin(), chosen_word.end(), chosen_word.begin(), ::tolower);
   for (Word & words: wordL) {
      check_word = words.word;
      //transform word to lowercase for catching as many matches as possible
      transform(check_word.begin(), check_word.end(), check_word.begin(), ::tolower);
      //make sure chosen word is from input
      if (check_word == chosen_word) {
         found = true;
         //print out the word
         cout << endl << "Word: " << words.word << endl;
         cout << "Chapters: ";
         //print out the words chapter metadata
         for (int & ch: words.ch) {
            if (ch != words.ch[words.ch.size() - 1]) {
               cout << ch << ", ";
            } else {
               cout << ch;
            }
         }
         cout << endl << "Lines: ";
         //print out the words line occurence metadata
         for (int & line: words.lines) {
            if (line != words.lines[words.lines.size() - 1]) {
               cout << line << ", ";
            } else {
               cout << line;
            }
         }
         //print out each line the word occurs on and each neighbour line
         cout << endl << endl;
         cout << "***************************************************************************" << endl;
         for (int & line: words.lines) {
            if (line < 2) {
               cout << line << " ";
               print_line(lines, (line - 1));
               cout << (line + 1) << " ";
               print_line(lines, line);
               cout << "***************************************************************************" << endl;
            } else {
               if (line >= lines.size()) {
                  cout << (line - 1) << " ";
                  print_line(lines, (line - 2));
                  cout << line << " ";
                  print_line(lines, (line - 1));
                  cout << "***************************************************************************" << endl;
               } else {
                  cout << (line - 1) << " ";
                  print_line(lines, (line - 2));
                  cout << line << " ";
                  print_line(lines, (line - 1));
                  cout << (line + 1) << " ";
                  print_line(lines, line);
                  cout << "***************************************************************************" << endl;
               }
            }
         }
         cout << endl;
      }
   }
   //if word wasn't found, let user know
   if (!found) cout << endl << "Word: " << word << ", was not found!" << endl << endl;
}

vector < vector < string >> process_ui_options(UI ui, double order) {
   vector < UI_Option > order_options;
   vector < vector < string >> processed_options;
   int processed_counter = 0;
   string last_type;
   int last_option_priority = 0;
   int counter = 0;
   for (UI_Option & option: ui.options.option) {
      if (option.display_order == order) {
         order_options.push_back(option);
      }
   }
   //sort by priority
   //sort by type
   for (UI_Option & option: order_options) {
      if (last_type.length() == 0) last_type == option.type;
      if(last_option_priority == 0)last_option_priority = option.priority;
      for (UI_Configs & config: ui.configs) {
         if (option.type == config.type) {
            if (last_type == option.type && counter != 0) {
               if (config.duplicate_handeling[0] == "none") {
                  continue;
               } else if (config.duplicate_handeling[0] == "merged") {
                  processed_options[processed_counter][1] += "," + option.name;
                  if (stoi(processed_options[processed_counter][2]) < option.priority){
                      processed_options[processed_counter][2] = option.priority;
                      last_option_priority = option.priority;
                  }
                  if (config.duplicate_handeling[1] == "yes") {
                     processed_options[processed_counter][3] += option.attributes[1];
                     if (config.duplicate_handeling[2].length() > 0) processed_options[processed_counter][3] += config.duplicate_handeling[2];
                     for (int i = 0; i < stoi(config.duplicate_handeling[3]); i++) {
                        processed_options[processed_counter][3] += '\n';
                     }
                  } else {
                     processed_options[processed_counter][3] += option.attributes[1];
                     if (config.duplicate_handeling[2].length() > 0) processed_options[processed_counter][3] += config.duplicate_handeling[2];
                  }
                  for (int i = 4; i < processed_options[processed_counter].size(); i++) {
                     processed_options[processed_counter][i] += "," + option.attributes[i - 2];
                  }
               } else {
                  last_option_priority = option.priority;
                  processed_options.push_back({
                     option.type,
                     option.name,
                     to_string(option.priority)
                  });
                  processed_counter++;
                  for (string & attribute: option.attributes) {
                     processed_options[processed_counter].push_back(attribute);
                  }
               }
            } else {
               last_option_priority = option.priority;
               processed_options.push_back({
                  option.type,
                  option.name,
                  to_string(option.priority)
               });
               processed_counter++;
               for (string & attribute: option.attributes) {
                  processed_options[processed_counter].push_back(attribute);
               }
            }
         }
      }
      counter++;
   }
   //sort by priority
   return processed_options;
}

void add_ui_option(UI ui, double display_order, int priority, string type, string name, vector < string > attributes) {
   UI_Option option;
   option.display_order = display_order;
   option.priority = priority;
   option.type = type;
   option.name = name;
   option.attributes = attributes;
   ui.options.option.push_back(option);
}

/*
 Run the program
*/
void run_program() {
   //keep track of words and their metadata
   vector < Word > wordL;
   //keep track of newlines from input
   vector <
      const char * > lines;
   //file to read
   string file = "../ch8 streams/test-words.txt";
   string input = "";
   //grab input from file
   read_file(file, input);
   //process input
   process_file_input(input, wordL, lines);
   UI ui;
   UI_Configs ui_config_selector;
   ui_config_selector.type = "selector";
   ui_config_selector.priority_order = 10;
   ui_config_selector.duplicate_handeling = {
      "merged",
      "no",
      ""
   };
   ui.configs.push_back(ui_config_selector);
   UI_Configs ui_config_input;
   ui_config_input.type = "input";
   ui_config_input.priority_order = 9;
   ui_config_input.duplicate_handeling = {
      "none"
   };
   ui.configs.push_back(ui_config_input);
   UI_Option ui_option_word;
   ui_option_word.display_order = 1.1;
   ui_option_word.priority = 1;
   ui_option_word.type = "selector";
   ui_option_word.name = "word";
   ui_option_word.attributes = {
      "",
      "s"
   };
   ui.options.option.push_back(ui_option_word);
   UI_Option ui_option_index;
   ui_option_index.display_order = 1.1;
   ui_option_index.priority = 1;
   ui_option_index.type = "selector";
   ui_option_index.name = "index";
   ui_option_index.attributes = {
      "",
      "I"
   };
   ui.options.option.push_back(ui_option_index);
   UI_Option ui_option_input;
   ui_option_index.display_order = 1.1;
   ui_option_index.priority = 1;
   ui_option_index.type = "input";
   ui_option_index.name = "input";
   ui_option_index.attributes = {
      ""
   };
   ui.options.option.push_back(ui_option_index);
   vector < vector < string >> processed_options;
   for (double order: ui.options.display_order) {
      processed_options = process_ui_options(ui, order);
      for (vector < string > option: processed_options) {
         for (UI_Configs config: ui.configs) {
            if (config.type == option[0]) {

            }
         }
      }
   }
}

int main() {
   //run program
   run_program();
   return 0;
}
