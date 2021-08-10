/******************************************************************************/
/*!
\file pch.h
\author Wong Swee Jong Nico
\par email: s.wong\@digipen.edu
\par DigiPen login: s.wong
\par Course: CS260-B
\par Assignment #04
\date 10/08/2021
\brief
This file contains an implementation of the functionality of a precompiled
header
*/
/******************************************************************************/
#pragma once
#pragma comment(lib, "ws2_32.lib")

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include "Windows.h"		// Entire Win32 API...
// #include "winsock2.h"	// ...or Winsock alone
#include "ws2tcpip.h"		// getaddrinfo()

//#include "ClientSender/ClientSender.h"

#include <vector>
#include <iostream>

#include <unordered_map>
#include <functional>
#include <string>
#include <ctype.h>

#include "Parser.h"
#include "Main.h"