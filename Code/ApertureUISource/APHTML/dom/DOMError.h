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

/// NOTE: The DLL/PCH Header should always be included last.
#include <APHTML/APEngineDLL.h>


namespace aperture::dom
{
  enum class DOMErrorType : unsigned short
  {
    DOM_NO_ERR = 0,
    DOM_INDEX_SIZE_ERR = 1,
    DOM_DOMSTRING_SIZE_ERR = 2,
    DOM_HIERARCHY_REQUEST_ERR = 3,
    DOM_WRONG_DOCUMENT_ERR = 4,
    DOM_INVALID_CHARACTER_ERR = 5,
    DOM_NO_DATA_ALLOWED_ERR = 6,
    DOM_NO_MODIFICATION_ALLOWED_ERR = 7,
    DOM_NOT_FOUND_ERR = 8,
    DOM_NOT_SUPPORTED_ERR = 9,
    DOM_INUSE_ATTRIBUTE_ERR = 10,
    DOM_INVALID_STATE_ERR = 11,
    DOM_SYNTAX_ERR = 12,
    DOM_INVALID_MODIFICATION_ERR = 13,
    DOM_NAMESPACE_ERR = 14,
    DOM_INVALID_ACCESS_ERR = 15,
    DOM_VALIDATION_ERR = 16,
    DOM_TYPE_MISMATCH_ERR = 17,
  };
  NS_DECLARE_POD_TYPE(DOMErrorType);
} // namespace aperture::dom
