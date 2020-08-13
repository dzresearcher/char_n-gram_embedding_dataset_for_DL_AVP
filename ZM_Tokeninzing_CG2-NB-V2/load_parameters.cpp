/**
  * Project         : ZM SOURCE CODE METRICS
  * File            : load_parameters.cpp
  * description     : implementation file for load_parameters module
  * date of creation: 06/11/2016
  * date of update  :
  * author          : Mohammed ZAGANE
  */

#include <fstream> // ifstream
#include <string> // string,..
#include <cstring> // strtok
#include "load_parameters.h"


//globals  variables
std::vector<std::string> String_Markup;
std::vector<std::string> Char_Markup;
std::vector<std::string> Befor_Str_Markup;
std::vector<std::string> Befor_Chr_Markup;
std::vector<std::string> Separators;
std::vector<std::string> Separators2;
std::vector<std::string> Separators3;
std::vector<std::string> Comment_Start;
std::vector<std::string> Comment_End;
std::vector<std::string> Comment_Line;
std::vector<std::string> Control_Commands;
std::vector<std::string> Integer_Number_Regex;
std::vector<std::string> Real_Number_Regex;
std::vector<std::string> Octal_Number_Regex;
std::vector<std::string> Hexadecimal_Number_Regex;
std::vector<std::string> Identifier_Regex;
std::vector<std::string> String_Regex;
std::vector<std::string> Char_Regex;
std::vector<std::string> Type_Specificators;
std::vector<std::string> Storage_Class_Specificators;
std::vector<std::string> Type_Qualificators;
std::vector<std::string> Key_Words;
std::vector<std::string> Operators;
std::vector<std::string> Begin_End;

//--------------------------
std::vector<std::string> Program_Log;


/**
  * this function load parameters from the configuration file (languages file)
  * Parameters :
  *   const char * Languages_File_Name : full path to the configuration (languages) file
  *   const char * Parameter : the parameter to load
  * Returned values :
  *   a std::string std::vector contains the list of parameter values , or an empty std::string std::vector if the function has field
  */
std::vector<std::string> Load_Language_Parameters(std::string Languages_File_Name, std::string Parameter)
{
        std::string Line, Token, Prmtr(Parameter);
        std::vector<std::string> Prmtr_Lst;
        std::ifstream Languages_File(Languages_File_Name);

        if (!Languages_File.good() )
        {
                return Prmtr_Lst; // unable to open the configuration (language file)
        }

        std::string Prmtr_First_Markup = "<";
        Prmtr_First_Markup += Parameter ;
        Prmtr_First_Markup += ">" ;

        std::string Prmtr_Second_Markup = "</";
        Prmtr_Second_Markup += Parameter ;
        Prmtr_Second_Markup += ">" ;


        bool Prmtr_First_Markup_Found = false;
        bool Prmtr_Second_Markup_Found = false;

        while (getline(Languages_File, Line) && !Prmtr_Second_Markup_Found)//process line by line
        {
                if( (Line.find(Prmtr_First_Markup) >=0) && (Line.find(Prmtr_First_Markup)<= Line.length()) )
                {
                        Prmtr_First_Markup_Found = true;
                        continue;
                }

                if( (Line.find(Prmtr_Second_Markup) >=0) && (Line.find(Prmtr_Second_Markup)<= Line.length()) )
                {
                        Prmtr_Second_Markup_Found = true;
                        continue;
                }

                if(Prmtr_First_Markup_Found)
                {

                        //tokenising

                        char * Temp =  (char *) Line.c_str();

                        char * Token2 = strtok (Temp, " "); // Token2 : c string
                        while (Token2)
                        {
                                Token = Token2; // string <-- cstring
                                Prmtr_Lst.insert(Prmtr_Lst.end(),Token);
                                Token2 = strtok (NULL, " ");
                        }
                }

        }

        Languages_File.close();
        return Prmtr_Lst;
}

int Load_All_Language_Parameters(std::string Language_File_Name)
{

    Clear_Vectors();
    Control_Commands = Load_Language_Parameters(Language_File_Name, "Control_Commands") ;
    String_Markup = Load_Language_Parameters(Language_File_Name , "String_Markup") ;
    Char_Markup = Load_Language_Parameters(Language_File_Name, "Char_Markup") ;
    Befor_Str_Markup = Load_Language_Parameters(Language_File_Name, "Befor_String_Markup") ;
    Befor_Chr_Markup = Load_Language_Parameters(Language_File_Name, "Befor_Char_Markup") ;
    Separators = Load_Language_Parameters(Language_File_Name, "Separators") ;
    Separators.insert(Separators.end(), " "); // white space and tab can't stored in the language file
    Separators.insert(Separators.end(), "\t");// because i use the space as a separator in this file
                                              // so i have to add them her manually
    Separators2 = Load_Language_Parameters(Language_File_Name, "Separators2") ;
    Separators3 = Load_Language_Parameters(Language_File_Name, "Separators3") ;
    Integer_Number_Regex = Load_Language_Parameters(Language_File_Name, "Integer_Number_Regex") ;
    Real_Number_Regex = Load_Language_Parameters(Language_File_Name, "Real_Number_Regex") ;
    Octal_Number_Regex = Load_Language_Parameters(Language_File_Name, "Octal_Number_Regex") ;
    Hexadecimal_Number_Regex = Load_Language_Parameters(Language_File_Name, "Hexadecimal_Number_Regex") ;
    Identifier_Regex = Load_Language_Parameters(Language_File_Name, "Identifier_Regex") ;
    String_Regex = Load_Language_Parameters(Language_File_Name, "String_Regex") ;
    Char_Regex = Load_Language_Parameters(Language_File_Name, "Char_Regex") ;
    Comment_Start = Load_Language_Parameters(Language_File_Name, "Comment_Start") ;
    Comment_End = Load_Language_Parameters(Language_File_Name, "Comment_End") ;
    Comment_Line = Load_Language_Parameters(Language_File_Name, "Comment_Line") ;
    Type_Specificators = Load_Language_Parameters(Language_File_Name, "Type_Specificators") ;
    Storage_Class_Specificators = Load_Language_Parameters(Language_File_Name, "Storage_Class_Specificators") ;
    Type_Qualificators = Load_Language_Parameters(Language_File_Name, "Type_Qualificators") ;
    Key_Words = Load_Language_Parameters(Language_File_Name, "Key_Words") ;
    Operators = Load_Language_Parameters(Language_File_Name, "Operators") ;
    Begin_End = Load_Language_Parameters(Language_File_Name, "Begin_End") ;

    return 0;
}

void Clear_Vectors()
{
    // clear vectors
    Control_Commands.clear();
    String_Markup.clear();
    Char_Markup.clear();
    Befor_Str_Markup.clear();
    Befor_Chr_Markup.clear();
    Separators.clear();
    Separators2.clear();
    Separators3.clear();
    Integer_Number_Regex.clear();
    Real_Number_Regex.clear();
    Octal_Number_Regex.clear();
    Hexadecimal_Number_Regex.clear();
    Identifier_Regex.clear();
    String_Regex.clear();
    Char_Regex.clear();
    Comment_Start.clear();
    Comment_End.clear();
    Comment_Line.clear();
    Type_Specificators.clear();
    Storage_Class_Specificators.clear();
    Type_Qualificators.clear();
    Key_Words.clear();
    Operators.clear();
    Begin_End.clear();

    Program_Log.clear();
}
