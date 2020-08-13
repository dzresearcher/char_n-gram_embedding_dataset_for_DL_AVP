/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   process_syse_cg.h
 * Author: zm
 *
 * Created on 11 avril 2019, 22:11
 */

#ifndef PROCESS_SYSE_CG_H_INCLUDED
#define PROCESS_SYSE_CG_H_INCLUDED


long Process_SYSE_CGD_File( std::string SYSE_File_Name);
std::string preProcess_Line( std::string Line);
long Process_SYSE_CGD_File2( std::string SYSE_File_Name, long Size_To_Save, bool Pre_Process_Lines);
long Process_SYSE_CGD_File2_ARFF( std::string SYSE_File_Name, long Size_To_Save, bool Pre_Process_Lines);

#endif /* PROCESS_SYSE_CG_HINCLUDED */

