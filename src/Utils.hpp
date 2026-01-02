#pragma once
#include <Geode/Geode.hpp>
#include <Geode/loader/SettingV3.hpp>

using namespace geode::prelude;

CCSprite* createIconSprite(std::string const& name);
bool getSwitchValue(std::string const& key);

struct TextureUtils {
    static bool g_isToolboxInit;
    
    static const std::unordered_map<int, std::pair<std::string, std::string>> iconMap;

    static void setObjIcon(EffectGameObject* obj, const std::string& texture);
    
    static void updateCompTexture(ItemTriggerGameObject* obj);
    static void updateEditTexture(ItemTriggerGameObject* obj, bool dot);
    static void updateSFXTexture(SFXTriggerGameObject* obj);
    static void updateUiTexture(UISettingsGameObject* obj);
    static void updateEventTexture(EventLinkTrigger* obj, float gap);
    static void updateStartTexture(StartPosObject* obj);

    static void updateOffsetCamTexture(CameraTriggerGameObject* obj, bool color);
    static void updateRotateCamTexture(CameraTriggerGameObject* obj, bool color);
    static void updateEdgeCamTexture(CameraTriggerGameObject* obj);

    struct DynamicSettings {
        bool ev, sfx, comp, ui, dotEdit, start, color, cam;
        float offEv;
    };
    static void applyDynamicUpdates(EffectGameObject* obj, const DynamicSettings& settings);
    
    static void applyDynamicChangesGlobal();
};
