/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "utils.h"


std::string& ltrim(std::string& str, const std::string& chars = "\t\n\v\f\r ")
{
    str.erase(0, str.find_first_not_of(chars));
    return str;
}
 
std::string& rtrim(std::string& str, const std::string& chars = "\t\n\v\f\r ")
{
    str.erase(str.find_last_not_of(chars) + 1);
    return str;
}
 
std::string& trim(std::string& str, const std::string& chars = "\t\n\v\f\r ")
{
    return ltrim(rtrim(str, chars), chars);
}

/**
  * This function preprocess lines from the original dataset, 
  * to remove line number from each line
  * 
  * Example :
    
    static void goodG2B() 85
    size_t data ; 87
    size_t * dataPtr2 = & data ; 89
    data = 0; 91

   * This will be :

    static void goodG2B()
    size_t data ;
    size_t * dataPtr2 = & data ;
    data = 0;
 */
std::string preProcess_Line( std::string Line)
{
    // to remove the line number, preprocessing is as follow:
    // from the end of the line we will search for the first "space"
    // when the "space" is found, store its position in line and 
    // do a substring
    
    int Index = -1;
    
    for (int i=Line.length()-1; i>=0 ; i--)
    {
        if (Line[i]== ' ')
        {
            Index = i;
            break;
        }
    }
    
    if ( (Index !=-1) && (Index > 0))
    {
        return Line.substr(0,Index);
    }
    else
    {
        return Line;
    }
    
}
/*
bool Is_String_Token(std::string Token)
{
    
    if (Token[0]=='"' && Token[Token.length() - 1]=='"')
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool Is_Char_Token(std::string Token)
{
    if (Token[0]=='\'' && Token[Token.length() - 1]=='\'')
    {
        return true;
    }
    else
    {
        return false;
    }
}

*/


