# v1.6.1
## Micro patch
- FPS message now appears only once.

# v1.6.0
## Dynamic Performance & Texture System Update
- Added **dynamic texture for StartPos** with **~2300 unique variations** depending on settings and context.
- Added **dynamic textures for Camera triggers**, reacting to their parameters.
- **Updated collision block textures** with clearer visuals and better editor readability.
- **Major code optimization**:
  - Reduced editor draw calls.
  - Optimized dynamic texture evaluation logic.
  - Lower memory usage for texture caching.
- General **code cleanup and refactoring** for better maintainability and stability.
- Added **FPS monitoring system**:
  - The mod now detects low FPS situations.
  - Displays a warning suggesting disabling certain dynamic textures to improve performance.

# v1.5.0
## Visual Refresh Update
- Added 5 completely new trigger sprites.
- Updated and replaced 25 old trigger sprites with new redesigned versions.
- Improved overall visual consistency between trigger categories.
- Minor tweaks to sprite alignment and scaling in the editor.

# v1.4.2
## Editor Interaction Fixes & Tweaks
- Added dynamic texture for Item Edit triggers.
- Added two new settings for Item Edit textures.
- Updated some trigger textures.
- Dynamic button now disappears during Playtest.

# v1.4.0
## Dynamic Texture Expansion
- Added new dynamic textures:
  - UI trigger
  - Item comp trigger
- Added new settings to configure dynamic textures:
  - Enable Item comp dynamic texture
  - Enable UI dynamic texture
- Added a button in the editor to toggle dynamic textures.

# v1.3.3
## Smart Event Composition
- Added a new feature: when an Event trigger contains multiple actions that cannot be represented by a single texture, the mod now automatically combines multiple textures into one.
- Added a new setting to enable or configure this texture-combination behavior.

# v1.3.2
## Visual Settings Overhaul
- Added *<cr>V</c><co>i</c><cy>S</c><cg>u</c><cj>A</c><cl>l</c>* settings 😎

# v1.3.1
## Trigger Sprite Settings
- Added new trigger textures:
  - Song
  - Song Edit
  - Shake
- Added new settings:
  - Reset StartPos trigger texture
  - Reset Shake trigger texture
  - Reset Area Stop trigger texture

# v1.30
## Dynamic Behavior Update
- Added dynamic texture updates for SFX and Event triggers:
  - SFX textures now change depending on volume.
  - Event textures now change based on their settings.
  - Settings allow disabling dynamic updates for SFX or Event triggers.
  - Added volume threshold setting for SFX triggers to control texture changes.
- Added several new textures for triggers that previously did not have custom visuals.

# v1.20
## Camera & Visual Additions
- Added new cameras trigger texture.
- Added several new trigger textures.
- Added new settings.

# v1.10
## Trigger visualizer
- Added new trigger textures:
  - Gameplay triggers
  - Area triggers
  - Logical triggers
- Added mod settings:
  - Toggle texture replacement per trigger category.
- Fixed shader trigger icon offset in the editor.
- Improved overall editor visual clarity.

# v1.0.1
## Platform Compatibility Patch
- Cross-platform release.

# v1.0.0
## Shader visualizer
- Initial release.