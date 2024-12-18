Aperture UI uses CMake. Currently, it is only working on Windows & PS5, and the build system is very messy and needs cleaning up(task/build-system-cleanup).

NOTE: OPEN CMD AND RUN THIS COMMAND: git submodule update --init & git submodule update --recursive


For now: This is what you need to build CMake:
BUILD_SAMPLES=ON
BUILD_SHARED_LIBS=ON (???, We build shared by default??? this will be assigned to bscleanup.)
NS_V8_ROOT=E:/InternalMiddleware/ApertureUI/Code/ThirdParty/v8/v8_prebuilt (Example, v8_prebuilt should always be last in the path, we currently only support the prebuilt versions of v8, source building is comming soon).
SAMPLES_BACKEND=GLFW_GL3 (Set this at your own discretion, some backends dont work with some shaders, so they will need to be fixed).
NS_V8_PREBUILT=ON (This is required for now).
NS_BUILD_EXPERIMENTAL_VULKAN=ON (Needed for linux, targets may be hidden, but are not needed unless your samples backend is using vulkan).
-DGLFW_BUILD_TEST=OFF -DURIPARSER_BUILD_TESTS=OFF -DURIPARSER_BUILD_TOOLS=OFF
-DURIPARSER_BUILD_DOCS=OFF

Aperture UI Uses a customized version of APHarrlow to render primitives. This is a submodule in the project. To build APHarrlow, you need to set the following:
APHARRLOW_BUILD_EXAMPLES=OFF (This is for the examples, not the samples, this is not needed for now).
APHARRLOW_DISABLE_TEXT_SUPPORT=OFF (This is for the text support, APFontEngine Handles the layout of texts (APHarrlow doesn't Handle Correctness of Hebrew & Arabic Languages Yet)).

Hermes is for mobile, but can work for consoles, which are experimental. 

Rest of the values should work for now.


