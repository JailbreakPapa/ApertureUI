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
#include <Foundation/Basics.h>
#include <cmath>
/// NOTE: The DLL/PCH Header should always be included last.
#include <APHTML/APEngineDLL.h>
namespace aperture::layout
{
/**
 * @brief The DPIUtils class provides utility functions for calculating DPI (dots per inch) and pixel density.
 */
class NS_APERTURE_DLL DPIUtils
{
private:
  double monitorWidth; /**< The width of the monitor in inches. */
  double monitorHeight; /**< The height of the monitor in inches. */
  double aspectRatio; /**< The aspect ratio of the monitor. */

public:
  /**
   * @brief Constructs a DPIUtils object with the specified monitor dimensions and aspect ratio.
   * @param width The width of the monitor in inches.
   * @param height The height of the monitor in inches.
   * @param ratio The aspect ratio of the monitor.
   */
  DPIUtils(double width, double height, double ratio)
  {
    monitorWidth = width;
    monitorHeight = height;
    aspectRatio = ratio;
  }

  /**
   * @brief Calculates the DPI (dots per inch) of the monitor.
   * @return The DPI of the monitor.
   */
  double calculateDPI()
  {
    double diagonalInches = nsMath::Sqrt(nsMath::Pow(monitorWidth, (double)2) + nsMath::Pow(monitorHeight, (double)2));
    double diagonalPixels = nsMath::Sqrt(nsMath::Pow(monitorWidth * aspectRatio, (double)2) + nsMath::Pow(monitorHeight, (double)2));
    double dpi = diagonalPixels / diagonalInches;
    return dpi;
  }

  /**
   * @brief Calculates the pixel density of a screen given its pixel dimensions and diagonal size.
   * @param pixelWidth The width of the screen in pixels.
   * @param pixelHeight The height of the screen in pixels.
   * @param screenDiagonal The diagonal size of the screen in inches.
   * @return The pixel density of the screen.
   */
  double calculatePixelDensity(int pixelWidth, int pixelHeight, double screenDiagonal)
  {
    // Use std::sqrt for this one...
    double diagonalPixels = std::sqrt(pixelWidth * pixelWidth + pixelHeight * pixelHeight);
    return diagonalPixels / screenDiagonal;
  }
};
}