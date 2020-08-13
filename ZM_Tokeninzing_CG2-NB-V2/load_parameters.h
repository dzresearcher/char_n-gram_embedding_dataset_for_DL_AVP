/**
  * Project         : ZM SOURCE CODE METRICS
  * File            : load_parameters.h
  * description     : header file for load_parameters module
  * date of creation: 06/11/2016
  * date of update  :
  * author          : Mohammed ZAGANE
  */


#ifndef LOAD_PARAMETERS_H_INCLUDED
#define LOAD_PARAMETERS_H_INCLUDED

#include <vector>
//using namespace std;

//globals  variables
extern std::vector<std::string> String_Markup; // c,c++, java : "" , pascal : ''
extern std::vector<std::string> Char_Markup; // c,c++ : ''
extern std::vector<std::string> Befor_Str_Markup; // c,c++ : \ example : "some character \" another char "
                                        // pascal: ' example : ' come character '' another char '
extern std::vector<std::string> Befor_Chr_Markup; // c,c++ : \ example : '\\'
extern std::vector<std::string> Separators; // , ; : + - * / & : < > % ......
extern std::vector<std::string> Separators2; // 2 chars : :: -- ++ << >> || ......
extern std::vector<std::string> Separators3; // 3 char : ... <<= >>=
extern std::vector<std::string> Integer_Number_Regex; // regular expression used to detect the integer numbers
extern std::vector<std::string> Real_Number_Regex; // regular expression used to detect the real numbers
extern std::vector<std::string> Octal_Number_Regex; // regular expression used to detect the octal numbers
extern std::vector<std::string> Hexadecimal_Number_Regex; // regular expression used to detect the hexadecimal numbers
extern std::vector<std::string> Identifier_Regex; // regular expression used to detect identificators
extern std::vector<std::string> String_Regex; // regular expression used to detect strings
extern std::vector<std::string> Char_Regex; // regular expression used to detect chars
extern std::vector<std::string> Comment_Start; // multi-lines comment first markup  : c++ /*
extern std::vector<std::string> Comment_End; // multi-lines comment second markup : c++ */
extern std::vector<std::string> Comment_Line; // one line comment markup : //
extern std::vector<std::string> Control_Commands; // commands used to calculates Mccab number ( cyclomatic complexity)
extern std::vector<std::string> Type_Specificators; // c++ : bool char double ....
                                        // pascal : integer real ....
extern std::vector<std::string> Storage_Class_Specificators; // c++ : auto extern inlin register static typedef virtual mtuable
extern std::vector<std::string> Type_Qualificators; // c++ : const friend volatile
extern std::vector<std::string> Key_Words; // the language keywords
extern std::vector<std::string> Operators; // the language operators ( arithmetics, boolean,.... )
extern std::vector<std::string> Begin_End; // Begin and End

//-------------------------------
extern std::vector<std::string> Program_Log; // the program log



// functions
std::vector<std::string> Load_Language_Parameters(std::string Languages_File_Name, std::string Parameter);
int Load_All_Language_Parameters(std::string Language);
void Clear_Vectors();


#endif // LOAD_PARAMETERS_H_INCLUDED
