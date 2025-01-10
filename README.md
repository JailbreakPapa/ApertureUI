# ApertureUI

ApertureUI is a HTML/CSS/JS UI Toolkit for video-games, real-time graphics applications and more.

Utilizing a light-weight framework, extensible, plugin based system, that allows modularity, and minimal dependency's, it can be used in many applications, where performance must stay a high priority.

The toolkit aims to be a open-source option for other Web-based UI Toolkits, like Coherent GameFace.

# Third Party Libraries

See thirdpartylicenses.txt for all of the used libraries in the SDK.

There are some Libraries that aren't compatible with the projects Build System(CMake):

- ICU
- V8
- JavaScript(Only Used For Platform Blue)

ICU should be installed by the user directly, either through some package manager like vcpkg.

We Try not use Libraries that don't build well with the SDK, but ICU is required for localization.

V8 & JavaScript are Handled by Default by the BuildSystem, and is built by the Repositories GHActions. check there for binary forms for V8.

## Platforms

The current supported platforms that Aperture can work with is Windows, PS4, PS5, Linux, FreeBSD, Nintendo Switch, Xbox One and Xbox Series X/S.

### Console Platforms (NDA)

Please contact us at: <consoles@wdstudios.tech> for access to console builds (after verification)

## License

ApertureUI is Open-Sourced Under the **MIT License**.

There is one requirement, when using ApertureUI, you are **Required** to Atrribute Aperture as Such: " (Your Product) uses Aperture UI. Copyright (Usage Year) (C) WD Studios L.L.C & Mikael K. Aboagye. All Rights Reserved " along with a banner of the ApertureUI Logo.

## Custom Licensing

You can also contact us for custom licensing.

For example, you may want to use ApertureUI in a embedded device, and don't feel like the standard license cant work for you, feel free to contact us, and we will attempt to reach a agreement that favors you.
