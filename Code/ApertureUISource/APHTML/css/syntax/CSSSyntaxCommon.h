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
#include <APHTML/APEngineCommonIncludes.h>

namespace aperture::css
{
    struct NS_APERTURE_DLL CommonSyntax
    {
        static const char LRBracket = '{';
        static const char RRBracket = '}';
        static const char LBracket = '(';
        static const char RBracket = ')';
        static const char LSBracket = '[';
        static const char RSBracket = ']';
        static const char Semicolon = ';';
        static const char Colon = ':';
        static const char Comma = ',';
        static const char Dot = '.';
        static const char Hash = '#';
        static const char At = '@';
        static const char Asterisk = '*';
        static const char Plus = '+';
        static const char Tilde = '~';
        static const char Caret = '^';
        static const char Dollar = '$';
        static const char Percent = '%';
        static const char Ampersand = '&';
        static const char Exclamation = '!';
        static const char Question = '?';
        static const char Equal = '=';
        static const char Slash = '/';
        static const char BackSlash = '\\';
        static const char Underscore = '_';
        static const char Hyphen = '-';
        static const char DoubleHyphen = '--';
        static const char DoubleQuote = '"';
        static const char SingleQuote = '\'';
        static const char NewLine = '\n';
        static const char Tab = '\t';
        static const char Space = ' ';
    };
}