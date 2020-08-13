/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   utils.h
 * Author: ZM
 *
 * Created on June 24, 2020, 8:04 PM
 */

#ifndef UTILS_H
#define UTILS_H

//--- for triming 
#include <algorithm> 
#include <functional> 
#include <cctype>
#include <locale>
//---

// trim from start (in place)
std::string& ltrim(std::string& str, const std::string& chars);
std::string& rtrim(std::string& str, const std::string& chars);
std::string& trim(std::string& str, const std::string& chars);
std::string preProcess_Line( std::string Line);
bool Is_String_Token(std::string Token);
bool Is_Char_Token(std::string Token);
#endif /* UTILS_H */

