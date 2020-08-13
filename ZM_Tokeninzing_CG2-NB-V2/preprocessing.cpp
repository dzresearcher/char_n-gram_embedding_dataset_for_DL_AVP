/**
  * Project         : ZM SOURCE CODE METRICS
  * File            : preprocessing.cpp
  * description     : implementation file for preprocessing
  * date of creation: 07/09/2016
  * date of update  : 21/06/2020
  * author          : Mohammed ZAGANE
  * description     : this file containe all function needed to preprocess code
  *                   like (eliminating space, comments,....)
  */

#include <fstream> // ifstream, ofstream
#include <algorithm> 
#include <functional> 
#include <cctype>
#include <locale>
#include "preprocessing.h"
#include "load_parameters.h"


/**
*  this function eliminate all space , tab from the begin of a line
*/



void Remove_Space_From_Line (std::string & Src_Line)
{
        //Src_Line = Src_Line.substr(Src_Line.find_first_not_of(' ') );
        //Src_Line = Src_Line.substr(Src_Line.find_first_not_of('\t') );
        bool Must_Delete = false;

        for (unsigned int i=0; i<=Src_Line.length();i++)
        {
                if((Src_Line[i]==' ' ||  Src_Line[i]=='\t'))
                {
                        if (Must_Delete == true)
                        {
                                Src_Line.erase(i,1);
                                i--;
                        }
                        else
                        {
                                Must_Delete = true;
                        }
                }
                else
                {
                        Must_Delete = false;
                }
        }
        return;
}


/**
* This function Eliminate space, tab from the begining of lines of code
*/
std::vector<std::string> Eliminate_Spaces (std::vector<std::string> Input_Code)
{
    std::string Line;// holde the line content
    std::vector<std::string> Output_Code;
    unsigned long Index =0;

    while (Index< Input_Code.size())
    {
        Line = Input_Code[Index];
        if(Line.length() != 0)// if the line isn't empty
        {
            Remove_Space_From_Line(Line);
            Output_Code.insert(Output_Code.end(),Line);
        }
        Index++;
    }
    return Output_Code;// function succefuly terminated
}

/**
*  this function eliminate all comments from the code
*/
std::vector<std::string> Eliminate_Comments (std::vector<std::string> Input_Code)
{
    bool Comment_Start_Found = false;
    std::string Src_Line;
    std::string Cmnt_Strt = Comment_Start[0];
    std::string Cmnt_End = Comment_End[0];
    std::string Cmnt_Line = Comment_Line[0];
    unsigned int Comment_Line_Position, Comment_Start_Position, Comment_End_Position;

    std::vector<std::string> Output_Code;
    unsigned long Index =0;
    while (Index< Input_Code.size())
    {
        Src_Line = Input_Code[Index];
        if( !Comment_Start_Found)//the first comment markup not yet found
        {
            Comment_Line_Position = Src_Line.find(Cmnt_Line);
            // if a one line comment markup found at the beginning of the line
            if( Comment_Line_Position ==0 )
            {
                Index++;
                continue; // skip the next instructions and  go to the next line
            }
            else if ( Comment_Line_Position >0  && Comment_Line_Position<= Src_Line.length())
            {
                Src_Line.erase(Src_Line.begin()+Comment_Line_Position, Src_Line.end());
            }

            Again :
            Comment_Start_Position = Src_Line.find(Cmnt_Strt);

            if((Comment_Start_Position >=0) && (Comment_Start_Position<= Src_Line.length()))
            {
                Comment_Start_Found = true;// indicate that the F C Mrkp Found
                Comment_End_Position = Src_Line.find(Cmnt_End);
                // if the S C Mrkp found in the same line ( /* comment */)
                if((Comment_End_Position >=0) && (Comment_End_Position<= Src_Line.length()))
                {
                    Comment_Start_Found = false;// this will indicate the end of the current comment
                    Src_Line.erase(Src_Line.begin()+ Comment_Start_Position,
                    Src_Line.begin()+ Comment_End_Position + Cmnt_End.length());
                    if(Src_Line.length()==0) // this happen where the line contains a multi-line comment in one line
                    {
                        Index++;
                        continue; // avoid to insert a blank line after the deletion of the comment
                    }
                    else
                    {
                        goto Again;// do many pass until there will no comment
                    }
                }
                else
                {
                    Index++;
                    continue;// skip adding line
                }
                // continue;
            }
        }
        else // the F C Mrkp already found
        {
            // if the S C Mrkp found
            if((Src_Line.find(Cmnt_End) >=0) && (Src_Line.find(Cmnt_End)<= Src_Line.length()))
            {
                Comment_Start_Found = false; // this will indicate the end of comment
            }
            Index++;
            continue;
        }
        Output_Code.insert(Output_Code.end(), Src_Line);
        Index++;
    }
    return Output_Code;//function successfully terminated
}

/**
* This function Eliminate comments, space, tab from the code
*/
std::vector<std::string> Extract_Code (std::vector<std::string> Input_Code)
{
    std::vector<std::string> Temp = Eliminate_Spaces(Input_Code);
    std::vector<std::string> Output_Code = Eliminate_Comments(Temp);
    Temp.clear();
    return Output_Code;// function succefuly terminated
}
