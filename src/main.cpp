#include "Utils.hpp"
#include <Geode/modify/EffectGameObject.hpp>
#include <Geode/modify/EditorUI.hpp>
#include <Geode/modify/SetupTriggerPopup.hpp>
#include <Geode/modify/LevelEditorLayer.hpp>
#include <Geode/modify/LevelSettingsObject.hpp>

// change texture
class $modify(MyEffectGameObject, EffectGameObject) {
    void customSetup() {
        EffectGameObject::customSetup();

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
            if (tex) TextureUtils::setObjIcon(this, tex);
        }

       if (!TextureUtils::g_isToolboxInit && Mod::get()->getSavedValue<bool>("dynamic", false)) {
            TextureUtils::DynamicSettings ds;
            ds.ev = getSwitchValue("dyn-ev");
            ds.sfx = getSwitchValue("dyn-sfx");
            ds.comp = getSwitchValue("dyn-comp");
            ds.ui = getSwitchValue("dyn-ui");
            ds.start = getSwitchValue("dyn-start");
            ds.dotEdit = getSwitchValue("dot-edit");
            ds.cam = getSwitchValue("dyn-cam");
            ds.color = getSwitchValue("color-cam");
            ds.offEv = Mod::get()->getSettingValue<float>("off-ev");
            
            TextureUtils::applyDynamicUpdates(this, ds);
        }
    }
};

void checkAndAlertFPS(cocos2d::CCNode* runner) {
    // Если предупреждение уже было показано, выходим и ничего не делаем
    if (Mod::get()->getSavedValue<bool>("fps-warned", false)) {
        return;
    }

    auto delay = CCDelayTime::create(0.5f);
    auto checkFPS = CallFuncExt::create([]() {
        float dt = CCDirector::sharedDirector()->getDeltaTime();
        
        // Проверяем FPS (меньше 20)
        if (dt > 0.0f && (1.0f / dt) < 20.0f) {
            // ЕЩЕ РАЗ проверяем перед показом
            if (Mod::get()->getSavedValue<bool>("fps-warned", false)) return;

            // Помечаем, что предупреждение показано
            Mod::get()->setSavedValue("fps-warned", true);

            FLAlertLayer::create(
                "Trigger visualizer",      
                "<cr>Your FPS is too low! </c>\n"
                "It is recommended to <cy>disable</c> some dynamic texture options "
                "in the mod settings to improve performance.",  
                "OK"                    
            )->show();
        }
    });

    if (runner) {
        runner->runAction(CCSequence::create(delay, checkFPS, nullptr));
    }
}

// Этот блок выполняется при загрузке мода
$execute {
    // Сбрасываем флаг при запуске игры
    Mod::get()->setSavedValue("fps-warned", false);

    // Слушаем изменение настройки "dynamic"
    // ИСПРАВЛЕНО: Используем глобальную функцию listenForSettingChanges
    listenForSettingChanges("dynamic", +[](bool value) {
        Mod::get()->setSavedValue("fps-warned", false);
    });
}

// add button + callback (dynamic texture)
class $modify(ShowDynamic, EditorUI) {
    bool init(LevelEditorLayer* editorlayer) {
        TextureUtils::g_isToolboxInit = true;
        
        if (!EditorUI::init(editorlayer)) {
            TextureUtils::g_isToolboxInit = false;
            return false;
        }

        TextureUtils::g_isToolboxInit = false;

        if (!getSwitchValue("dyn-ev") && !getSwitchValue("dyn-sfx") && 
            !getSwitchValue("dyn-comp") && !getSwitchValue("dyn-ui") && !getSwitchValue("dyn-start")) return true;

        bool isDynamic = Mod::get()->getSavedValue<bool>("dynamic", false);

        auto menu = this->getChildByID("undo-menu");
        if (menu) {
            auto baseSpr = CCSprite::create("dynamicoff.png"_spr);
            if (baseSpr) {
                auto onSpr = CCSprite::create("dynamicon.png"_spr);
                if (onSpr) {
                    onSpr->setPosition(baseSpr->getContentSize() / 2);
                    onSpr->setVisible(isDynamic);
                    onSpr->setID("on-indicator");
                    baseSpr->addChild(onSpr);
                }
                baseSpr->setScale(0.6f);

                auto btn = CCMenuItemSpriteExtra::create(baseSpr, nullptr, this, menu_selector(ShowDynamic::onButton));
                btn->setID("dynamic-button");
                menu->addChild(btn);
                menu->updateLayout();
            }
        }

        if (isDynamic) {
            TextureUtils::applyDynamicChangesGlobal();
        }
        return true;
    }

    void onButton(CCObject* sender) {
        bool current = Mod::get()->getSavedValue<bool>("dynamic", false);
        bool newValue = !current;
        Mod::get()->setSavedValue<bool>("dynamic", newValue);

        Mod::get()->setSavedValue("fps-warned", false);

        auto btn = static_cast<CCMenuItemSpriteExtra*>(sender);
        if (auto onSpr = btn->getNormalImage()->getChildByID("on-indicator")) {
            onSpr->setVisible(newValue);
        }

        if (newValue) {
            TextureUtils::applyDynamicChangesGlobal();
            
            checkAndAlertFPS(this);

        } else {
            resetDynamicIcons();
        }
    }
    void resetDynamicIcons() {
        auto lel = LevelEditorLayer::get();
        if (!lel || !lel->m_objects) return;

        Ref<CCArray> arr = lel->m_objects;
        for (auto obj : CCArrayExt<EffectGameObject*>(arr)) {
            if (!obj) continue;
            int id = obj->m_objectID;

            if (id == 31) {
                if (getSwitchValue("do-default") && !getSwitchValue("new-start")) {
                    TextureUtils::setObjIcon(obj, "start.png"_spr);
                }
                continue; 
            }

            if (id == 1916 || id == 2015 || id == 2062 || id == 3602 || id == 3604 || id == 3619 || id == 3620 || id == 3613) {
                 auto it = TextureUtils::iconMap.find(id);
                 if (it != TextureUtils::iconMap.end()) {
                     TextureUtils::setObjIcon(obj, it->second.first);
                 }
            }
        }
    }

    void onDeselectAll(CCObject* sender) {
        EditorUI::onDeselectAll(sender);
        TextureUtils::applyDynamicChangesGlobal();

        if (Mod::get()->getSavedValue<bool>("dynamic", false)) {

            checkAndAlertFPS(this);
        }
    }
    

    void onPlaytest(CCObject* sender) {
        if (auto m = getChildByID("undo-menu")) {
            if (auto b = m->getChildByID("dynamic-button")) b->setVisible(false);
        }
        EditorUI::onPlaytest(sender);
    }

    void onStopPlaytest(CCObject* sender) {
        if (auto m = getChildByID("undo-menu")) {
            if (auto b = m->getChildByID("dynamic-button")) b->setVisible(true);
        }
        EditorUI::onStopPlaytest(sender);
    }
};



// update dynamic texture on close popup
class $modify(SetupTriggerPopup) {
    void onClose(cocos2d::CCObject* sender) {
        SetupTriggerPopup::onClose(sender); 
        
        TextureUtils::applyDynamicChangesGlobal();

        if (Mod::get()->getSavedValue<bool>("dynamic", false)) {
             if (auto lel = LevelEditorLayer::get()) {
                checkAndAlertFPS(lel);
             }
        }
    }
};