#include "Utils.hpp"
#include <Geode/modify/EffectGameObject.hpp>
#include <Geode/modify/EditorUI.hpp>
#include <Geode/modify/SetupTriggerPopup.hpp>
#include <Geode/modify/LevelEditorLayer.hpp>
#include <Geode/modify/LevelSettingsObject.hpp>
#include <Geode/modify/LevelSettingsLayer.hpp>
#include <Geode/modify/SetupCameraOffsetTrigger.hpp>
#include <Geode/modify/SetupCameraModePopup.hpp>
#include <Geode/modify/ColorSelectPopup.hpp>


static bool s_dynamicReady = false;

static bool isModEnabled() {
    return getSwitchValue("new-triggers");
}

static void refreshLevelIcons() {
    auto lel = LevelEditorLayer::get();
    if (!lel || !lel->m_objects) {
        return;
    }

    Ref<CCArray> arr = lel->m_objects;
    for (auto obj : CCArrayExt<EffectGameObject*>(arr)) {
        if (!obj) {
            continue;
        }
        obj->customSetup();
    }

    TextureUtils::clearDynamicCache();
    if (isModEnabled() && getSwitchValue("dyn-enable")) {
        TextureUtils::applyDynamicChangesGlobal();
    }
}

static void applyDynamicToObjects(CCArray* objects) {
    if (!objects) {
        return;
    }
    if (!isModEnabled() || !s_dynamicReady || !getSwitchValue("dyn-enable")) {
        return;
    }

    auto ds = TextureUtils::getDynamicSettings();
    for (auto obj : CCArrayExt<GameObject*>(objects)) {
        if (auto eff = typeinfo_cast<EffectGameObject*>(obj)) {
            TextureUtils::applyDynamicUpdatesCached(eff, ds);
        }
    }
}

// change texture
class $modify(MyEffectGameObject, EffectGameObject) {
    void customSetup() {
        EffectGameObject::customSetup();

        if (!isModEnabled()) {
            return;
        }

        int id = m_objectID;

        auto it = TextureUtils::iconMap.find(id);
        if (it != TextureUtils::iconMap.end()) {
            if (getSwitchValue(it->second.second)) {
                TextureUtils::setObjIcon(this, it->second.first);
            }
        }

        if (id == 31 && getSwitchValue("do-default") && !getSwitchValue("new-start")) {
            TextureUtils::setObjIcon(this, "start.png"_spr);
        }

        if (getSwitchValue("do-area")) {
             if (id == 3024) {
                 TextureUtils::setObjIcon(this, getSwitchValue("color-stop") ? "astopc.png"_spr : "astop.png"_spr);
             } else if (id == 3023) {
                 TextureUtils::setObjIcon(this, getSwitchValue("color-stop") ? "estopc.png"_spr : "estop.png"_spr);
             }
        }

        if (getSwitchValue("do-cam")) {
            bool colorCam = getSwitchValue("color-cam");
            const char* tex = nullptr;
            switch (id) {
                case 1913: tex = colorCam ? "zoom.png"_spr : "czoom.png"_spr; break;
                case 1914: tex = colorCam ? "static.png"_spr : "cstatic.png"_spr; break;
                case 1916: tex = colorCam ? "offset.png"_spr : "Coffset.png"_spr; break;
                case 2015: tex = colorCam ? "rotatecam.png"_spr : "crotate.png"_spr; break;
                case 2062: tex = "edge.png"_spr; break;
                case 2925: tex = "mode.png"_spr; break;
                case 2016: tex = "guide.png"_spr; break;
                case 1520: if (!getSwitchValue("new-shake")) tex = "shake.png"_spr; break;
            }
            if (tex) {
                TextureUtils::setObjIcon(this, tex);
            }
        }

       if (!TextureUtils::g_isToolboxInit && s_dynamicReady && getSwitchValue("dyn-enable")) {
            auto ds = TextureUtils::getDynamicSettings();
            TextureUtils::applyDynamicUpdatesCached(this, ds);
        }
    }
};

// dynamic texture apply (create, copy...)
class $modify(ShowDynamic, EditorUI) {
    bool init(LevelEditorLayer* editorlayer) {
        TextureUtils::g_isToolboxInit = true;
        
        if (!EditorUI::init(editorlayer)) {
            TextureUtils::g_isToolboxInit = false;
            return false;
        }

        TextureUtils::g_isToolboxInit = false;

        if (!isModEnabled()) {
            s_dynamicReady = false;
            return true;
        }

        if (!getSwitchValue("dyn-logic") && !getSwitchValue("dyn-cam") &&
            !getSwitchValue("dyn-game") && !getSwitchValue("dyn-color")) {
            return true;
        }

        s_dynamicReady = false;
        this->runAction(CCSequence::create(
            CCDelayTime::create(0.0f),
            CallFuncExt::create([]() {
                s_dynamicReady = true;
                TextureUtils::clearDynamicCache();
                TextureUtils::applyDynamicChangesGlobal();
            }),
            nullptr
        ));
        return true;
    }

    GameObject* createObject(int objectID, cocos2d::CCPoint position) {
        auto obj = EditorUI::createObject(objectID, position);
        if (!obj) {
            return obj;
        }
        if (!isModEnabled() || !s_dynamicReady || !getSwitchValue("dyn-enable")) {
            return obj;
        }

        if (auto eff = typeinfo_cast<EffectGameObject*>(obj)) {
            auto ds = TextureUtils::getDynamicSettings();
            TextureUtils::applyDynamicUpdatesCached(eff, ds);
        }
        return obj;
    }

    void onDuplicate(CCObject* sender) {
        EditorUI::onDuplicate(sender);
        applyDynamicToObjects(this->getSelectedObjects());
    }

    void onPaste(CCObject* sender) {
        EditorUI::onPaste(sender);
        applyDynamicToObjects(this->getSelectedObjects());
    }


    void onDeselectAll(CCObject* sender) {
        EditorUI::onDeselectAll(sender);
        if (!isModEnabled()) {
            return;
        }
        TextureUtils::applyDynamicChangesGlobal();
    }
};

$execute {
    if (auto mod = Mod::get()) {
        listenForSettingChanges<bool>("new-triggers", [](bool) {
            refreshLevelIcons();
        }, mod);

        listenForSettingChanges<bool>("dyn-enable", [](bool value) {
            if (!isModEnabled()) {
                TextureUtils::clearDynamicCache();
                refreshLevelIcons();
                return;
            }

            if (value) {
                TextureUtils::clearDynamicCache();
            }
            refreshLevelIcons();
        }, mod);
    } else {
    }
}


class $modify(MySetupTriggerPopup, SetupTriggerPopup) {
    void onClose(cocos2d::CCObject* sender) {
        SetupTriggerPopup::onClose(sender); 
        if (!isModEnabled()) {
            return;
        }
        
        TextureUtils::markDynamicDirty(this->m_gameObject);
        TextureUtils::markDynamicDirty(this->m_gameObjects);
        TextureUtils::applyDynamicChangesGlobal();
    }
};

class $modify(MySetupCameraOffsetTrigger, SetupCameraOffsetTrigger) {
    void onClose(cocos2d::CCObject* sender) {
        SetupCameraOffsetTrigger::onClose(sender);
        if (!isModEnabled()) {
            return;
        }

        TextureUtils::markDynamicDirty(this->m_gameObject);
        TextureUtils::markDynamicDirty(this->m_gameObjects);
        TextureUtils::applyDynamicChangesGlobal();
    }
};

class $modify(MyColorSelectPopup, ColorSelectPopup) {
    void applyColorPopupDynamicUpdate() {
        if (!isModEnabled()) {
            return;
        }
        TextureUtils::markDynamicDirty(this->m_gameObject);
        TextureUtils::markDynamicDirty(this->m_gameObjects);
        TextureUtils::markDynamicDirty(this->m_colorObjects);
        TextureUtils::applyDynamicChangesGlobal();
    }

    void onClose(cocos2d::CCObject* sender) {
        ColorSelectPopup::onClose(sender);
        applyColorPopupDynamicUpdate();
    }

    void closeColorSelect(cocos2d::CCObject* sender) {
        ColorSelectPopup::closeColorSelect(sender);
        applyColorPopupDynamicUpdate();
    }
};

class $modify(MyLevelSettingsLayer, LevelSettingsLayer) {
    void onClose(cocos2d::CCObject* sender) {
        LevelSettingsLayer::onClose(sender);
        if (!isModEnabled()) {
            return;
        }

        if (auto lel = LevelEditorLayer::get()) {
            if (auto objects = lel->m_objects) {
                for (auto obj : CCArrayExt<EffectGameObject*>(objects)) {
                    if (obj && obj->m_objectID == 31) {
                        TextureUtils::markDynamicDirty(obj);
                    }
                }
            }
        }
        TextureUtils::applyDynamicChangesGlobal();
    }
};
