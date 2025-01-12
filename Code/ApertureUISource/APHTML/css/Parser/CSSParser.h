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
#include <boost/parser/parser.hpp>
#include <fstream>
#include <ostream>
namespace bp = boost::parser;

namespace aperture::css::parser
{
  using standard_iterator = std::string::const_iterator;
  using foundation_interation = nsStringIterator;

  
  /// @brief CSS Parser Error Handler, modified to store log messages in a buffer instead of a file.
  struct css_logging_error_handler
  {
    css_logging_error_handler() = default;

    css_logging_error_handler(const char* in_filename)
    {
      filename = in_filename;
    }
    // Store error messages in a buffer instead of a file
    template <typename Iter, typename Sentinel>
    bp::error_handler_result operator()(Iter first, Sentinel last, bp::parse_error<Iter> const& e) const
    {
      std::ostringstream oss;
      bp::write_formatted_expectation_failure_error_message(oss, "buffer", first, last, e);
      log_message(oss.str());
      return bp::error_handler_result::fail;
    }

    template <typename Context, typename Iter>
    void diagnose(
      bp::diagnostic_kind kind,
      std::string_view message,
      Context const& context,
      Iter it) const
    {
      std::ostringstream oss;
      bp::write_formatted_message(oss, "buffer", bp::_begin(context), it, bp::_end(context), message);
      log_message(oss.str());
    }

    template <typename Context>
    void diagnose(
      bp::diagnostic_kind kind,
      std::string_view message,
      Context const& context) const
    {
      diagnose(kind, message, context, bp::_where(context).begin());
    }

    // Add a new log message to the buffer
    void log_message(const std::string& message) const
    {
      log_buffer.push_back(message);
    }

    // Access log messages for inspection
    const std::vector<std::string>& get_log() const
    {
      return log_buffer;
    }

  private:
    const char* filename;
    mutable std::vector<std::string> log_buffer; // Store log messages
  };
} // namespace aperture::css::parser
