/**
  * Project         : ZM SOURCE CODE METRICS
  * File            : tokeniser.h
  * description     : header file for tokeniser module
  * date of creation: 04/11/2016
  * date of update  :
  * author          : Mohammed ZAGANE
  */

#ifndef TOKENISER_H_INCLUDED
#define TOKENISER_H_INCLUDED
#include <vector>// vector


// functions
std::vector<std::string> Generate_Tokens_List (std::vector<std::string> Input_Code);
std::vector<std::string> Separate_Strings(std::vector<std::string> Input_Code);
std::vector<std::string> Separate_Chars(std::vector<std::string> First_Pass_Tokens);
std::vector<std::string> Separate_Numbers (std::vector<std::string> Second_Pass_Tokens);
std::vector<std::string> Generate_Tokens_List_Fourth_Pass (std::vector<std::string> Third_Pass_Tokens);
bool Is_A_String_Token (std::string Line);
bool Is_A_Char_Token (std::string Line);
bool Is_A_Number_Token (std::string Line);



#endif // TOKENISER_H_INCLUDED
