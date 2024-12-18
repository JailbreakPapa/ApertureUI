#include <APHTML/APEngineCommonIncludes.h>
#include <APHTML/core/BaseElement.h>
#include <APHTML/core/SCTypes.h>

namespace aperture
{
  class NS_APERTURE_DLL ScrollController
  {
  public:
    enum class Mode
    {
      None,
      Smoothscroll, // Smooth scrolling to target distance.
      Autoscroll,   // Scrolling with middle mouse button.
    };

    void ActivateAutoscroll(Element* target, nsVec2 start_position);

	void ActivateSmoothscroll(Element* target, nsVec2 delta_distance, ScrollBehavior scroll_behavior);

	bool Update(nsVec2 mouse_position, float dp_ratio);

	void IncrementSmoothscrollTarget(nsVec2 delta_distance);

	// Resets any active mode and its state.
	void Reset();

	// Sets the scroll behavior for mouse wheel processing and scrollbar interaction.
	void SetDefaultScrollBehavior(ScrollBehavior scroll_behavior, float speed_factor);

	// Returns the autoscroll cursor based on the active scroll velocity.
	nsString GetAutoscrollCursor(nsVec2 mouse_position, float dp_ratio) const;
    
	// Returns true if autoscroll mode is active and the cursor has been moved outside the idle scroll area.
	bool HasAutoscrollMoved() const;

	Mode GetMode() const { return mode; }
	Element* GetTarget() const { return target; }
    private:
	// Updates time to now, and returns the delta time since the previous time update.
	float UpdateTime();

	void UpdateAutoscroll(nsVec2 mouse_position, float dp_ratio);

	void UpdateSmoothscroll(float dp_ratio);

	bool HasSmoothscrollReachedTarget() const;

	void PerformScrollOnTarget(nsVec2 delta_distance);

	Mode mode = Mode::None;

	Element* target = nullptr;
	double previous_update_time = 0;

	nsVec2 autoscroll_start_position;
	nsVec2 autoscroll_accumulated_length;
	bool autoscroll_moved = false;

	bool smoothscroll_prefer_instant = false;
	float smoothscroll_speed_factor = 1.f;

	nsVec2 smoothscroll_target_distance;
	nsVec2 smoothscroll_scrolled_distance;
  };
} // namespace aperture