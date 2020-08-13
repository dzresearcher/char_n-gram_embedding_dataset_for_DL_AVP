/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ZM_SCM_SySe_CG.cpp
 * Author: Mohammed ZAGANE
 *
 * Created on 11 avril 2019, 16:44
 * Updated : 04/07/2019
 */

#include <iostream> // cout, cin,...
#include <fstream> // for : ofstream
#include "load_parameters.h" // Load_All_Language_Parameters(...)
#include "process_syse_cg.h" // for : 





//--------------------------------------

static void Show_Usage(std::string Program_Name)
{
    std::cerr << "Usage: " << Program_Name << " [Path_To_Code_gadgets_Dataset] Label_Type" 
              << std::endl
              << "Examples :"
              << std::endl
              << Program_Name << " Code_gadgets_DDCD_for_training.txt FASTTEXT"
              << std::endl
              << Program_Name << " c:\\dataset\\Code_gadgets_DDCD_for_training.txt ARFF"
              << std::endl
              << Program_Name << " dataset\\Code_gadgets_DDCD_for_training.txt ARFF"
              << std::endl;        
}

/**
  *
  * Program entry
  *
  */

int main(int argc, char** argv) // the main function
{
    std::string Error_Message="";
    
    if (argc < 3) {
        
        //Process_SYSE_CGD_File( "Code_gadgets_DD_for_testing.txt");
        Show_Usage("ZM_Tokenizing_Syse_CG_Dataset");
        
        
        
        return 1;
    }
    else
    {
        try
        {
               
            std::string Language_File_Name, Language;
            Language = "cpp";

            //Win
            Language_File_Name = "languages//" + Language + ".lng";

            std::cout << "Loading Language parameters from : "<< std::endl;
            std::cout << Language_File_Name;
            std::cout << "..........";
        
            if (Load_All_Language_Parameters(Language_File_Name) == 0)
            {
                std::cout << "OK"<< std::endl;
            }
            else
            {
                std::cout << "FAILD!"<< std::endl;
                Error_Message = "Unable to load configuration !!! ):";
                goto Exist_Faild;
            }
            std::vector<std::string> Files_To_Processe;
            Files_To_Processe.insert(Files_To_Processe.end(), argv[1] );
           /* Files_To_Processe.insert(Files_To_Processe.end(), "Original_Dataset//DDCD_dataset//Code_gadgets_DDCD_for_testing.txt" );
        
            Files_To_Processe.insert(Files_To_Processe.end(), "Original_Dataset//DD_dataset//Code_gadgets_DD_for_training.txt" );
            */
            
       
            long Nbr_Inst;
            for (int j=0; j<Files_To_Processe.size(); j++ )
            {
                std::cout << "Processing file :"<< Files_To_Processe[j]<< "..."<< std::endl;
                if (std::string(argv[2]) == "ARFF")
                {
                    std::cout << "(ARFF) Processing file :"<< Files_To_Processe[j]<< "... " << std::endl;
                    //Nbr_Inst = Process_SYSE_CGD_File2_ARFF(Files_To_Processe[j], 1000, true);
                    Nbr_Inst = Process_SYSE_CGD_File2_ARFF(Files_To_Processe[j], 1000, false);
                }
                else
                {
                    std::cout << " (FASTTEXT) Processing file :"<< Files_To_Processe[j]<< "..."<< std::endl;
                    //Nbr_Inst = Process_SYSE_CGD_File2(Files_To_Processe[j], 1000, true);
                    Nbr_Inst = Process_SYSE_CGD_File2(Files_To_Processe[j], 1000, false);
                }
                
                
                if (Nbr_Inst == -1)
                {
                    std::cout << "FAILD!"<< std::endl;
                    Error_Message = "Unable to open SYSE/CGD file !!! ):";
                }
                else if (Nbr_Inst == -2)
                {
                    std::cout << "FAILD!"<< std::endl;
                    Error_Message = "Unable to create Tokens files !!! ):";
                }
                else
                {
                    std::cout << "DONE! "<< Nbr_Inst << " instances generated."<< std::endl;
                }
            
                std::cout << "All files processed :) "<<  std::endl;
            }
            std::cout << "DONE! "<< Nbr_Inst << " instances generated."<< std::endl;
        }
        catch (...)
        {
            std::cout<< " Unknown ERROR !!!";
        }

        Clear_Vectors(); // from load_parameters
        return 0;


        Exist_Faild:
        {
            std::cout << "ERROR !! :"<< std::endl;
            std::cout << Error_Message<< std::endl;
            return -1;
        }
    }
}
