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

#include <APHTML/dom/DOMElement.h>

#include <string>
/// NOTE: The DLL/PCH Header should always be included last.
#include <APHTML/APEngineDLL.h>

namespace aperture
{
  namespace dom
  {
    class NS_APERTURE_DLL DOMCDataSection : public DOMNode
    {
      NS_ALLOW_PRIVATE_PROPERTIES(aperture::dom::DOMCDataSection);
    public:
      DOMCDataSection(const nsString& data)
        : data_(data)
      {
      }

      // Comparison operators (compares wrapped node pointers)
      bool operator==(const DOMCDataSection& r) const
      {
        return data_.IsEqual(r.data_);
      }

      bool operator!=(const DOMCDataSection& r) const
      {
        return data_.IsEqualN(r.data_,r.data_.GetCharacterCount());
      }

      bool operator<(const DOMCDataSection& r) const
      {
        return data_.Compare(r.data_);
      }

      bool operator>(const DOMCDataSection& r) const
      {
        return data_.CompareN(r.data_, r.data_.GetCharacterCount());
      }

      bool operator<=(const DOMCDataSection& r) const
      {
        return data_.Compare_NoCase(r.data_);
      }

      bool operator>=(const DOMCDataSection& r) const
      {
        return data_.CompareN_NoCase(r.data_,r.data_.GetCharacterCount());
      }

    private:
      nsString data_;
    };
  } // namespace dom
} // namespace aperture
// Path: DOMCDataSection.h

NS_DECLARE_REFLECTABLE_TYPE(NS_APERTURE_DLL, aperture::dom::DOMCDataSection);
