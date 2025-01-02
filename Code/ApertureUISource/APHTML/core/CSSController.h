#include <APHTML/css/CSSErrorDB.h>

namespace aperture
{
  /// @brief The CSS controller for the Aperture Engine. This class is responsible for managing the CSS parser and the CSS error database.
  /// @note you can access cached CSS data from this class, check errors, and more.
  class NS_APERTURE_DLL CSSController
  {
  public:
    inline static css::CSSErrorDatabase& GetErrorDatabase() { return m_errorDB; }
    // TODO: Add CSSAST Tree here.
  private:
    /// @brief The error database for the CSS controller.
    inline static css::CSSErrorDatabase m_errorDB;
  };
} // namespace aperture
