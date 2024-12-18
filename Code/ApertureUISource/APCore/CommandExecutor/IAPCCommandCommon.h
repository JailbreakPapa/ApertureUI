/*
This code is part of Aperture UI - A HTML/CSS/JS UI Middleware

Copyright (c) 2020-2024 WD Studios L.L.C. and/or its licensors. All
rights reserved in all media.

The coded instructions, statements, computer programs, and/or related
material (collectively the "Data") in these files contain confidential
and unpublished information proprietary WD Studios and/or its
licensors, which is protected by United States of America federal
copyright law and by international treaties.

This software or source code is supplied under the terms of a license
agreement and nondisclosure agreement with WD Studios L.L.C. and may
not be copied, disclosed, or exploited except in accordance with the
terms of that agreement. The Data may not be disclosed or distributed to
third parties, in whole or in part, without the prior written consent of
WD Studios L.L.C..

WD STUDIOS MAKES NO REPRESENTATION ABOUT THE SUITABILITY OF THIS
SOURCE CODE FOR ANY PURPOSE. THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT
HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
MERCHANTABILITY, NONINFRINGEMENT, AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER, ITS AFFILIATES,
PARENT COMPANIES, LICENSORS, SUPPLIERS, OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
ANY WAY OUT OF THE USE OR PERFORMANCE OF THIS SOFTWARE OR SOURCE CODE,
EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#pragma once

#include <ostream>
#include <stdexcept>
#include <string>

namespace aperture::core
{
  enum class CommandType
  {
    Unknown,
    CSS,
    Composition,
    Layout,
    Rendering,
    Presentation,
    Custom
  };

  enum class Runtype
  {
    AnyThread,
    FreeThread_CSS,
    FreeThread_Composition,
    FreeThread_Layout,
    FreeThread_Rendering,
    FreeThread_Scripting,
    FreeThread_Presentation,
    FreeThread_Custom
  };

  // Conversion functions for CommandType
  static NS_ALWAYS_INLINE const char* CommandTypeToString(CommandType type)
  {
    switch (type)
    {
      case CommandType::Unknown:
        return "Unknown";
      case CommandType::CSS:
        return "CSS";
      case CommandType::Composition:
        return "Composition";
      case CommandType::Layout:
        return "Layout";
      case CommandType::Rendering:
        return "Rendering";
      case CommandType::Presentation:
        return "Presentation";
      case CommandType::Custom:
        return "Custom";
      default:
        throw std::invalid_argument("Invalid CommandType");
    }
  }

  static NS_ALWAYS_INLINE CommandType StringToCommandType(const char* str)
  {
    if (str == "Unknown")
      return CommandType::Unknown;
    if (str == "CSS")
      return CommandType::CSS;
    if (str == "Composition")
      return CommandType::Composition;
    if (str == "Layout")
      return CommandType::Layout;
    if (str == "Rendering")
      return CommandType::Rendering;
    if (str == "Presentation")
      return CommandType::Presentation;
    if (str == "Custom")
      return CommandType::Custom;
    throw std::invalid_argument("Invalid CommandType string");
  }

  // Conversion functions for Runtype
  static NS_ALWAYS_INLINE const char* RuntypeToString(Runtype type)
  {
    switch (type)
    {
      case Runtype::AnyThread:
        return "AnyThread";
      case Runtype::FreeThread_CSS:
        return "FreeThread_CSS";
      case Runtype::FreeThread_Composition:
        return "FreeThread_Composition";
      case Runtype::FreeThread_Layout:
        return "FreeThread_Layout";
      case Runtype::FreeThread_Rendering:
        return "FreeThread_Rendering";
      case Runtype::FreeThread_Presentation:
        return "FreeThread_Presentation";
      case Runtype::FreeThread_Custom:
        return "FreeThread_Custom";
      default:
        return "FreeThread_Custom";
    }
  }

  static NS_ALWAYS_INLINE Runtype StringToRuntype(const std::string& str)
  {
    if (str == "AnyThread")
      return Runtype::AnyThread;
    if (str == "FreeThread_CSS")
      return Runtype::FreeThread_CSS;
    if (str == "FreeThread_Composition")
      return Runtype::FreeThread_Composition;
    if (str == "FreeThread_Layout")
      return Runtype::FreeThread_Layout;
    if (str == "FreeThread_Rendering")
      return Runtype::FreeThread_Rendering;
    if (str == "FreeThread_Presentation")
      return Runtype::FreeThread_Presentation;
    if (str == "FreeThread_Custom")
      return Runtype::FreeThread_Custom;
    if (str.empty())
    {
      return Runtype::FreeThread_Custom;
    }
    throw std::invalid_argument("Invalid Runtype string");
  }

  // Overload operator<< for CommandType
  static NS_ALWAYS_INLINE std::ostream& operator<<(std::ostream& os, CommandType type)
  {
    os << CommandTypeToString(type);
    return os;
  }

  // Overload operator<< for Runtype
  static NS_ALWAYS_INLINE std::ostream& operator<<(std::ostream& os, Runtype type)
  {
    os << RuntypeToString(type);
    return os;
  }
} // namespace aperture::core
