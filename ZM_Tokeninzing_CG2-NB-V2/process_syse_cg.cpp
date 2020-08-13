/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   process_syse_cg.cpp
 * Author: Mohammed ZAGANE
 *
 * Created on 11 avril 2019, 22:08
 * Updated : 27/06/2020
 */

#include <dirent.h>//working with directory
#include <fstream>//ifstream , ofstream
#include <sstream>//ifstream , ofstream
#include <vector>// vector
#include <iostream>



#include "process_syse_cg.h"
#include "tokeniser.h"
#include "preprocessing.h"
#include "utils.h"



/**
  * This function do just like the first version do
  * except it progressevely save  'syses' by block 
  * in the destination file, this is useful for big files
  * Parameters :
  * std::string SYSE_File_Name : originale file (syse dataset)
  * long Size_To_Save : size of SYSE block to save each time
  * bool Pre_Process_Lines : to enable or desable preprocessing ( desabling preprocess lines is needed in case of dataset2)
  */
long Process_SYSE_CGD_File2( std::string SYSE_File_Name, long Size_To_Save, bool Pre_Process_Lines)
{
    std::ifstream SYSE_File (SYSE_File_Name.c_str()); // <! SYSE file pointer
    std::vector<std::string> SYSE_Src_Code; // <! SYSE source code , as a strings vector
    std::vector<std::string> Labeled_SYSE_Tokens_Lines; // <! Labeled Tokens calculated from the SYSE source, in arff format
    std::vector<std::string> Unlabeled_SYSE_Tokens_Lines; // <! Unlabeled Tokens calculated from the SYSE source, in arff format
    std::string Line; // <! line read from SYSE File
        
    //bool Begin_SYSE = false; // <! indicate the begin of a SYSE part when true
    long End_SYSE = true; // <! indicate the end of a SYSE part when true
    long Index = 0; // <! to control the actual beging of SYSE (only code to skip syse header) 
    std::string SYSE_Class_Label; // <! the class label of an instance (SYSE) in the arff file 
                                  // <! yes : vulnerable , no : clean
    std::string Labeled_Tokens_Line; // <! Tokens line (instance)x    
    std::string Unlabeled_Tokens_Line; // <! Tokens line (instance)x    
    
    long SYSE_Nbr = 0;
            
    std::stringstream Str_Buffer;

    /*Str_Buffer << SYSE_File.rdbuf();
    */
    
    
    std::ofstream Labeled_Tokens_File( (SYSE_File_Name + "_Labeled_Tokens.txt").c_str() );
    std::ofstream Unlabeled_Tokens_File( (SYSE_File_Name+ "_Unlabeled_Tokens.txt").c_str());
    
    long Nbr_Generated_SYSE = 0;
    
    if ( Labeled_Tokens_File.good() && Unlabeled_Tokens_File.good() )
    {

    }
    else
    {
        return -2;//unable to create tokens file
    }
    
    
    if(SYSE_File.good()) // SYSE file succefully opened
    {
        
        // loading the SYSE/CGD file in a string stream ------------------------
        // to avoid reading line by line directely from the file
        // wish slowdown executing
        SYSE_File.seekg(0,std::ios::end);
        std::streampos SYSE_File_Length = SYSE_File.tellg();
        SYSE_File.seekg(0,std::ios::beg);
        std::vector<char>  SYSE_File_Buffer(SYSE_File_Length);
        SYSE_File.read(&SYSE_File_Buffer[0],SYSE_File_Length);
        Str_Buffer.rdbuf()->pubsetbuf(&SYSE_File_Buffer[0],SYSE_File_Length);
        //---------- from stackoverflow.com ------------------------------------
               
        while (getline(Str_Buffer, Line)) // reading lines from the Str stream
        {
            if (!End_SYSE) // there are some lines remains in the Str stream
            {
                if( Line == "0" || Line== "1" || (Line[0]=='0' && Line.length()==2) || (Line[0]=='1' && Line.length()==2)) // the last line (wish indicate the class label)
                {                   
                    End_SYSE = true;
                    Index = 0;
                    if (Line == "1" || (Line[0]=='1' && Line.length()==2) )
                        SYSE_Class_Label = "__label__VULNERABLE";
                    if (Line == "0" || (Line[0]=='0' && Line.length()==2))
                        SYSE_Class_Label = "__label__CLEAN";
                    
                                       
                    std::vector<std::string> Tokens = Generate_Tokens_List (SYSE_Src_Code);
                    
                    Labeled_Tokens_Line = SYSE_Class_Label + " ";
                    Unlabeled_Tokens_Line = "";
                    
                    for (long i=0; i<Tokens.size(); i++)
                    {
                        // final token processing
                        // more processing can be added here , for example:
                        // replacing strings and chars with generic tokens ,....
                        
                        if ( Is_A_String_Token(Tokens[i]) )
                        {
                            
                            int Match_Length = Tokens[i].length();
                            if (Match_Length == 2) // "" 
                            {
                                Tokens[i] = "\"EMPTY_STRING\"";
                            }
                            else if (Match_Length > 2 && Match_Length < 5)
                            {
                                Tokens[i] = "\"SHORT_STRING\"";
                            }
                            else if (Match_Length>5 && Match_Length<15)
                            {
                                Tokens[i] = "\"MEDIUM_STRING\"";
                            }
                            else if (Match_Length > 15)
                            {
                                Tokens[i] = "\"LONG_STRING\"";
                            }            
                            
                        }
                        
                        if ( Is_A_Char_Token(Tokens[i]) )
                        {
                            Tokens[i] = "\"CHAR\"";
                        }
                        
                        // replacing (") by ('), to be suitable for arff format 
                        // in that format, token are introduced as string
                        // example : "token1 token2 token3",class_label
                        std::replace( Tokens[i].begin(), Tokens[i].end(), '"', '\'');// replace (") by (')
                        //-------- 
                                                
                        Labeled_Tokens_Line = Labeled_Tokens_Line + Tokens[i]+ " ";
                        Unlabeled_Tokens_Line = Unlabeled_Tokens_Line  + Tokens[i] + " ";
                    }
                                            
                    Labeled_SYSE_Tokens_Lines.insert(Labeled_SYSE_Tokens_Lines.end(), Labeled_Tokens_Line);
                    Unlabeled_SYSE_Tokens_Lines.insert(Unlabeled_SYSE_Tokens_Lines.end(), Unlabeled_Tokens_Line);
                    Nbr_Generated_SYSE ++;
                    SYSE_Src_Code.clear();
                                        
                    if (Labeled_SYSE_Tokens_Lines.size() == Size_To_Save)
                    {
                        for (long p=0; p<  Labeled_SYSE_Tokens_Lines.size(); p++)
                        {
                            Labeled_Tokens_File << Labeled_SYSE_Tokens_Lines[p] << std::endl;
                            Unlabeled_Tokens_File << Unlabeled_SYSE_Tokens_Lines[p] << std::endl;
                        }
                        
                        Labeled_Tokens_File.flush();
                        Unlabeled_Tokens_File.flush();
                        Labeled_SYSE_Tokens_Lines.clear();
                        Unlabeled_SYSE_Tokens_Lines.clear();
                        std::cout<< "\n" << Size_To_Save << " saved!"<< std::endl;
                    }    
                }
                else
                {
                    if (Pre_Process_Lines)  
                    {
                        //--- preprocess line to remove line number 
                        Line = preProcess_Line(Line);
                        //---- end --------------------------------
                    }
                         
                    SYSE_Src_Code.insert(SYSE_Src_Code.end(), Line);
                        
                }
            }
            else
            {
                Index++;
   
                if (Index == 2)
                {
                    End_SYSE = false;
                    Index =0;
                    SYSE_Nbr++;
                    //std::cout << "SYSE N°= "<< SYSE_Nbr << " :.......OK"<< std::endl ;
                }
            }
        }
    }
    else
    {
        return -1;// unable to open the SYSE file
    }
    
    
    
    if (Labeled_SYSE_Tokens_Lines.size() > 0) // write any remaining data
    {
        for (long p=0; p<  Labeled_SYSE_Tokens_Lines.size(); p++)
        {
            Labeled_Tokens_File << Labeled_SYSE_Tokens_Lines[p] << std::endl;
            Unlabeled_Tokens_File << Unlabeled_SYSE_Tokens_Lines[p] << std::endl;
        }
        Labeled_Tokens_File.flush();
        Unlabeled_Tokens_File.flush();
        Labeled_SYSE_Tokens_Lines.clear();
        Unlabeled_SYSE_Tokens_Lines.clear();
    }
    
    SYSE_Src_Code.clear();
    Str_Buffer.clear();
    
    Labeled_Tokens_File.close();
    Unlabeled_Tokens_File.close();
    SYSE_File.close();
    return Nbr_Generated_SYSE;
}

long Process_SYSE_CGD_File2_ARFF( std::string SYSE_File_Name, long Size_To_Save, bool Pre_Process_Lines)
{
    std::ifstream SYSE_File (SYSE_File_Name.c_str()); // <! SYSE file pointer
    std::vector<std::string> SYSE_Src_Code; // <! SYSE source code , as a strings vector
    std::vector<std::string> Labeled_SYSE_Tokens_Lines; // <! Labeled Tokens calculated from the SYSE source, in arff form
    std::string Line; // <! line read from SYSE File
        
    //bool Begin_SYSE = false; // <! indicate the begin of a SYSE part when true
    long End_SYSE = true; // <! indicate the end of a SYSE part when true
    long Index = 0; // <! to control the actual beging of SYSE (only code to skip syse header) 
    std::string SYSE_Class_Label; // <! the class label of an instance (SYSE) in the arff file 
                                  // <! yes : vulnerable , no : clean
    std::string Labeled_Tokens_Line; // <! Tokens line (instance)x    
       
    long SYSE_Nbr = 0;
            
    std::stringstream Str_Buffer;

    std::ofstream Labeled_Tokens_File( (SYSE_File_Name + "_Labeled_Tokens.arff").c_str() );    
    long Nbr_Generated_SYSE = 0;
    
    if ( Labeled_Tokens_File.good())
    {

    }
    else
    {
        return -2;//unable to create tokens file
    }
    
    
    if(SYSE_File.good()) // SYSE file succefully opened
    {
        
        // loading the SYSE/CGD file in a string stream ------------------------
        // to avoid reading line by line directely from the file
        // wish slowdown executing
        SYSE_File.seekg(0,std::ios::end);
        std::streampos SYSE_File_Length = SYSE_File.tellg();
        SYSE_File.seekg(0,std::ios::beg);
        std::vector<char>  SYSE_File_Buffer(SYSE_File_Length);
        SYSE_File.read(&SYSE_File_Buffer[0],SYSE_File_Length);
        Str_Buffer.rdbuf()->pubsetbuf(&SYSE_File_Buffer[0],SYSE_File_Length);
        //---------- from stackoverflow.com ------------------------------------
               
        while (getline(Str_Buffer, Line)) // reading lines from the Str stream
        {
            if (!End_SYSE) // there are some lines remains in the Str stream
            {
                if( Line == "0" || Line== "1" || (Line[0]=='0' && Line.length()==2) || (Line[0]=='1' && Line.length()==2)) // the last line (wish indicate the class label)
                {                   
                    End_SYSE = true;
                    Index = 0;
                    if (Line == "1" || (Line[0]=='1' && Line.length()==2) )
                        SYSE_Class_Label = "VULNERABLE";
                    if (Line == "0" || (Line[0]=='0' && Line.length()==2))
                        SYSE_Class_Label = "CLEAN";
                    
                    std::vector<std::string> Tokens = Generate_Tokens_List (SYSE_Src_Code);
                    
                    Labeled_Tokens_Line ="\"";
                    
                    for (long i=0; i<Tokens.size(); i++)
                    {
                        // final token processing
                        // more processing can be added here , for example:
                        // replacing strings and chars with generic tokens ,....
                        
                        if ( Is_A_String_Token(Tokens[i]) )
                        {
                            
                            int Match_Length = Tokens[i].length();
                            if (Match_Length == 2) // "" 
                            {
                                Tokens[i] = "\"EMPTY_STRING\"";
                            }
                            else if (Match_Length > 2 && Match_Length < 5)
                            {
                                Tokens[i] = "\"SHORT_STRING\"";
                            }
                            else if (Match_Length>5 && Match_Length<15)
                            {
                                Tokens[i] = "\"MEDIUM_STRING\"";
                            }
                            else if (Match_Length > 15)
                            {
                                Tokens[i] = "\"LONG_STRING\"";
                            }            
                            
                        }
                        
                        if ( Is_A_Char_Token(Tokens[i]) )
                        {
                            Tokens[i] = "\"CHAR\"";
                        }
                        
                        // replacing (") by ('), to be suitable for arff format 
                        // in that format, token are introduced as string
                        // example : "token1 token2 token3",class_label
                        std::replace( Tokens[i].begin(), Tokens[i].end(), '"', '\'');// replace (") by (')
                        //-------- 
                                                
                        Labeled_Tokens_Line = Labeled_Tokens_Line + Tokens[i]+ " ";
                    }
                    Labeled_Tokens_Line = Labeled_Tokens_Line + "\"" + "," + SYSE_Class_Label;
                                            
                    Labeled_SYSE_Tokens_Lines.insert(Labeled_SYSE_Tokens_Lines.end(), Labeled_Tokens_Line);
                    Nbr_Generated_SYSE ++;
                    SYSE_Src_Code.clear();
                    
                    if (Labeled_SYSE_Tokens_Lines.size() == Size_To_Save)
                    {
                        for (long p=0; p<  Labeled_SYSE_Tokens_Lines.size(); p++)
                        {
                            Labeled_Tokens_File << Labeled_SYSE_Tokens_Lines[p] << std::endl;
                        }
                        
                        Labeled_Tokens_File.flush();
                        Labeled_SYSE_Tokens_Lines.clear();
                        std::cout<< "\n" << Size_To_Save << " saved!"<< std::endl;
                    }    
                }
                else
                {
                    if (Pre_Process_Lines)  
                    {
                        Line = preProcess_Line(Line);
                    }
                         
                    SYSE_Src_Code.insert(SYSE_Src_Code.end(), Line);
                        
                }
            }
            else
            {
                /*if (SYSE_Nbr == 2)
                {
                    break;
                }
                */
                Index++;
   
                if (Index == 2)
                {
                    End_SYSE = false;
                    Index =0;
                    SYSE_Nbr++;
                    //std::cout << "SYSE N°= "<< SYSE_Nbr << " :.......OK"<< std::endl ;
                }
            }
        }
    }
    else
    {
        return -1;// unable to open the SYSE file
    }
    
    
    
    if (Labeled_SYSE_Tokens_Lines.size() > 0) // write any remaining data
    {
        for (long p=0; p<  Labeled_SYSE_Tokens_Lines.size(); p++)
        {
            Labeled_Tokens_File << Labeled_SYSE_Tokens_Lines[p] << std::endl;
        }
        Labeled_Tokens_File.flush();
        Labeled_SYSE_Tokens_Lines.clear();
    }
    
    SYSE_Src_Code.clear();
    Str_Buffer.clear();
    
    Labeled_Tokens_File.close();
    SYSE_File.close();
    return Nbr_Generated_SYSE;
}


