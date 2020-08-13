/**
Project         : ZM SOURCE CODE METRICS
File            : preprocessing.h
description     : header file for preprocessing
date of creation: 07/09/2016
date of update  : 03/01/2017
author          : Mohammed ZAGANE
*/


#ifndef PREPROCESSING_H_INCLUDED
#define PREPROCESSING_H_INCLUDED

#include <string>
#include <vector>// vector

void Remove_Space_From_Line(std::string & Src_Line);
std::vector<std::string> Eliminate_Spaces (std::vector<std::string> Input_Code);//
std::vector<std::string> Eliminate_Comments (std::vector<std::string> Input_Code);
std::vector<std::string> Extract_Code (std::vector<std::string> Input_Code);



#endif // PREPROCESSING_H_INCLUDED
