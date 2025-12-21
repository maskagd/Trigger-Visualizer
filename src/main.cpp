#include <Geode/Geode.hpp>
#include <Geode/modify/EffectGameObject.hpp>
#include <Geode/modify/CCSpriteBatchNode.hpp>
#include <Geode/modify/EditorUI.hpp>
#include <Geode/modify/CCSprite.hpp>
#include <Geode/modify/SetupSFXPopup.hpp>
#include <Geode/binding/SFXTriggerGameObject.hpp>
#include <Geode/binding/ToggleTriggerAction.hpp>
#include <Geode/modify/SetupEventLinkPopup.hpp>
#include <Geode/modify/SetupMoveCommandPopup.hpp>
#include <Geode/binding/EventLinkTrigger.hpp>
#include <Geode/modify/SetupTriggerPopup.hpp>
#include <Geode/loader/SettingV3.hpp>
#include <Geode/ui/Popup.hpp>

using namespace geode::prelude;

CCSprite* createIconSprite(std::string const& name) {
    auto modPath = Mod::get()->expandSpriteName(name.c_str());
    
    if (auto spr = CCSprite::create(std::string(modPath).c_str())) {
        return spr;
    }
    if (auto spr = CCSprite::createWithSpriteFrameName(name.c_str())) {
        return spr;
    }
    if (auto spr = CCSprite::create(name.c_str())) {
        return spr;
    }
    return CCSprite::create(); 
}

class SpriteSwitchSettingV3 : public SettingBaseValueV3<bool> {
public:
    std::string m_spr1; 
    std::string m_spr2; 

    static Result<std::shared_ptr<SettingV3>> parse(std::string const& key, std::string const& modID, matjson::Value const& json) {
        auto res = std::make_shared<SpriteSwitchSettingV3>();
        auto root = checkJson(json, "SpriteSwitchSettingV3");
        
        res->parseBaseProperties(key, modID, root);
        
        root.has("spr1").into(res->m_spr1);
        root.has("spr2").into(res->m_spr2);

        return root.ok(std::static_pointer_cast<SettingV3>(res));
    }

    SettingNodeV3* createNode(float width) override;
};

class SpriteSwitchNodeV3 : public SettingValueNodeV3<SpriteSwitchSettingV3> {
protected:
    bool init(std::shared_ptr<SpriteSwitchSettingV3> setting, float width) {
        if (!SettingValueNodeV3::init(setting, width))
            return false;
        
        auto menu = this->getButtonMenu();
        menu->removeAllChildren();

        auto btn1Spr = createIconSprite(setting->m_spr1);
        btn1Spr->setScale(0.6f); 
        auto btn1 = CCMenuItemSpriteExtra::create(btn1Spr, this, menu_selector(SpriteSwitchNodeV3::onSelectFalse));
        btn1->setTag(0); 
        
        auto btn2Spr = createIconSprite(setting->m_spr2);
        btn2Spr->setScale(0.6f);
        auto btn2 = CCMenuItemSpriteExtra::create(btn2Spr, this, menu_selector(SpriteSwitchNodeV3::onSelectTrue));
        btn2->setTag(1); 

        menu->addChild(btn1);
        menu->addChild(btn2);
        
        auto layout = RowLayout::create();
        layout->setGap(10.f); 
        layout->setAxisAlignment(AxisAlignment::End); 
        
        menu->setLayout(layout);
        menu->setContentSize({ 90.f, 40.f }); 
        menu->updateLayout();

        this->updateState(nullptr);
        return true;
    }

    void onSelectFalse(CCObject*) {
        this->setValue(false, nullptr);
    }

    void onSelectTrue(CCObject*) {
        this->setValue(true, nullptr);
    }

    void updateState(CCNode* invoker) override {
        SettingValueNodeV3::updateState(invoker);
        auto val = this->getValue();

        auto menu = this->getButtonMenu();
        if (auto btn1 = static_cast<CCMenuItemSpriteExtra*>(menu->getChildByTag(0))) {
            btn1->setColor(val ? ccGRAY : ccWHITE);
            btn1->setOpacity(val ? 120 : 255);
        }
        if (auto btn2 = static_cast<CCMenuItemSpriteExtra*>(menu->getChildByTag(1))) {
            btn2->setColor(val ? ccWHITE : ccGRAY);
            btn2->setOpacity(val ? 255 : 120);
        }
    }

public:
    static SpriteSwitchNodeV3* create(std::shared_ptr<SpriteSwitchSettingV3> setting, float width) {
        auto ret = new SpriteSwitchNodeV3();
        if (ret->init(setting, width)) {
            ret->autorelease();
            return ret;
        }
        delete ret;
        return nullptr;
    }
};

SettingNodeV3* SpriteSwitchSettingV3::createNode(float width) {
    return SpriteSwitchNodeV3::create(std::static_pointer_cast<SpriteSwitchSettingV3>(shared_from_this()), width);
}


bool getSwitchValue(std::string const& key) {
    auto setting = Mod::get()->getSetting(key);
    
    if (auto mySetting = std::dynamic_pointer_cast<SpriteSwitchSettingV3>(setting)) {
        return mySetting->getValue();
    }
    
    return false;
}

$execute {
    (void)Mod::get()->registerCustomSettingType("sprite-switch", &SpriteSwitchSettingV3::parse);
}

class $modify(SetupTriggerPopup) {
    void onClose(cocos2d::CCObject* sender) {
        this->applyChangesToObjects();
        SetupTriggerPopup::onClose(sender);
    }

     void applyChangesToObjects() {

		auto offset_ev = Mod::get()->getSettingValue<float>("off-ev");

		auto ev = getSwitchValue("dyn-ev");

		if (!ev) return;

        auto lel = LevelEditorLayer::get();
        if (!lel) return;

        auto arr = lel->m_objects;
        if (!arr) return;

        for (int i = 0; i < arr->count(); i++) {
            auto obj = static_cast<EffectGameObject*>(arr->objectAtIndex(i));
            if (!obj || obj->m_objectID != 3604) continue;

            updateObjectTexture(typeinfo_cast<EventLinkTrigger*>(obj), offset_ev);
        }
    }

	static void updateObjectTexture(EventLinkTrigger* obj, float gap = 10.f) {
		if (!obj) return;

		const auto& eids = obj->m_eventIDs;
		
		std::vector<const char*> singleTextures;
		std::vector<const char*> combinedTextures;

		auto addTex = [&](const char* s, const char* c) {
			if (std::find(singleTextures.begin(), singleTextures.end(), s) == singleTextures.end()) {
				singleTextures.push_back(s);
				combinedTextures.push_back(c);
			}
		};

		if (eids.empty()) {
			addTex("ev.png"_spr, "evs.png"_spr);
		} else {
			for (int id : eids) {
				if (id >= 1 && id <= 5) addTex("evland.png"_spr, "evlands.png"_spr);
				else if (id == 6) addTex("evhit.png"_spr, "evhits.png"_spr);
				else if ((id == 7 || id == 8) || (id >= 34 && id <= 44)) addTex("evorb.png"_spr, "evorbs.png"_spr);
				else if ((id == 9) || (id >= 45 && id <= 49)) addTex("evpad.png"_spr, "evpads.png"_spr);
				else if ((id == 10 || id == 11) || (id >= 50 && id <= 52)) addTex("evgravity.png"_spr, "evgravitys.png"_spr);
				else if (id >= 12 && id <= 22) addTex("evjump.png"_spr, "evjumps.png"_spr);
				else if (id == 62 || id == 63) addTex("evcoin.png"_spr, "evcoins.png"_spr);
				else if (id >= 65 && id <= 68) addTex("evfall.png"_spr, "evfalls.png"_spr);
				else if (id == 69) addTex("evtop.png"_spr, "evtops.png"_spr);
				else if (id == 70) addTex("evup.png"_spr, "evups.png"_spr);
				else if (id == 71) addTex("evleft.png"_spr, "evlefts.png"_spr);
				else if (id == 72) addTex("evlef.png"_spr, "evlefs.png"_spr);
				else if (id == 73) addTex("evright.png"_spr, "evrights.png"_spr);
				else if (id == 74) addTex("evrig.png"_spr, "evrigs.png"_spr);
				else if (id == 60 || id == 64) addTex("evsave.png"_spr, "evsaves.png"_spr);
				else if ((id >= 26 && id <= 33) || (id >= 50 && id <= 59)) addTex("evportal.png"_spr, "evportals.png"_spr);
				else addTex("ev.png"_spr, "evs.png"_spr);
			}
		}

		if (singleTextures.size() == 1) {
			if (auto spr = CCSprite::create(singleTextures[0])) {
				obj->setTexture(spr->getTexture());
				obj->setTextureRect(spr->getTextureRect());
			}
			return;
		}

		const char* tex1 = combinedTextures[0];
		const char* tex2 = combinedTextures[1];
		const char* textTex = "evtitles.png"_spr;

		auto spr1 = CCSprite::create(tex1);
		auto spr2 = CCSprite::create(tex2);
		auto sprText = CCSprite::create(textTex);

		if (!spr1 || !spr2 || !sprText) return;

		float totalWidth = 100.0f; 
		float totalHeight = 50.0f;
		float cx = totalWidth / 2.0f;
		float cy = totalHeight / 2.0f;

		spr1->setFlipY(true);
		spr2->setFlipY(true);
		sprText->setFlipY(true);

		sprText->setPosition({cx, cy});
		spr1->setPosition({cx - gap, cy}); 
		spr2->setPosition({cx + gap, cy});

		auto rt = CCRenderTexture::create(totalWidth, totalHeight);
		if (!rt) return;

		rt->beginWithClear(0, 0, 0, 0);
		spr1->visit();
		spr2->visit();
		sprText->visit();
		rt->end();

		auto tex = rt->getSprite()->getTexture();
		
		ccTexParams tp = {GL_LINEAR, GL_LINEAR, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE};
		tex->setTexParameters(&tp);

		obj->setTexture(tex);
		obj->setTextureRect({0, 0, totalWidth, totalHeight});
	}
};

class $modify(SetupSFXPopup) {
    void onClose(cocos2d::CCObject* sender) {
        this->applyChangesToObjects();
        SetupSFXPopup::onClose(sender);
    }

     void applyChangesToObjects() {
		
		auto sfx = getSwitchValue("dyn-sfx");

		if (!sfx) return;

        auto lel = LevelEditorLayer::get();
        if (!lel) return;

        auto arr = lel->m_objects;
        if (!arr) return;

        for (int i = 0; i < arr->count(); i++) {
            auto obj = static_cast<EffectGameObject*>(arr->objectAtIndex(i));
            if (!obj || obj->m_objectID != 3602) continue;

            updateObjectTexture(typeinfo_cast<SFXTriggerGameObject*>(obj));
        }
    }

    static void updateObjectTexture(SFXTriggerGameObject* obj) {
        if (!obj) return;

        float volume = obj->m_volume;

        const char* texture = nullptr;

		auto sfx2 = Mod::get()->getSettingValue<float>("sfx2");
		auto sfx3 = Mod::get()->getSettingValue<float>("sfx3");
		auto sfx4 = Mod::get()->getSettingValue<float>("sfx4");
		auto sfx5 = Mod::get()->getSettingValue<float>("sfx5");

        if (volume > sfx5) texture = "sfx5.png"_spr;
        else if (volume > sfx4) texture = "sfx4.png"_spr;
        else if (volume > sfx3) texture = "sfx3.png"_spr;
		else if (volume > sfx2) texture = "sfx2.png"_spr;
        else texture = "sfx1.png"_spr;

        if (CCSprite* spr = CCSprite::create(texture)) {
            obj->setTexture(spr->getTexture());
            obj->setTextureRect(spr->getTextureRect());
        }
    }
};

class $modify(MyEffectGameObject, EffectGameObject) {
	void customSetup() {
		EffectGameObject::customSetup();

		auto defalt = getSwitchValue("do-default");
        auto logic = getSwitchValue("do-logic");
        auto shader = getSwitchValue("do-shader");
        auto area = getSwitchValue("do-area");
        auto colis = getSwitchValue("do-colis");
        auto ccolor = getSwitchValue("color-cam");
        auto cam = getSwitchValue("do-cam");
        auto startp = getSwitchValue("new-start");
        auto cstop = getSwitchValue("color-stop");
        auto shakep = getSwitchValue("new-shake");

		if (shader == true)
		switch(m_objectID){

			case 2904: {
				setIcon("shader.png"_spr);
				break;
			}
			case 2905: {
				setIcon("shock.png"_spr);
				break;
			}
			case 2907: {
				setIcon("line.png"_spr);
				break;
			}
			case 2909: {
				setIcon("glitch.png"_spr);
				break;
			}
			case 2910: {
				setIcon("chromatic.png"_spr);
				break;
			}
			case 2911: {
				setIcon("chrglitch.png"_spr);
				break;
			}
			case 2912: {
				setIcon("pixelate.png"_spr);
				break;
			}
			case 2913: {
				setIcon("circles.png"_spr);
				break;
			}
			case 2914: {
				setIcon("radial.png"_spr);
				break;
			}
			case 2915: {
				setIcon("motion.png"_spr);
				break;
			}
			case 2916: {
				setIcon("bulge.png"_spr);
				break;
			}
			case 2917: {
				setIcon("pinch.png"_spr);
				break;
			}
			case 2919: {
				setIcon("blind.png"_spr);
				break;
			}
			case 2920: {
				setIcon("sepia.png"_spr);
				break;
			}
			case 2921: {
				setIcon("negative.png"_spr);
				break;
			}
			case 2922: {
				setIcon("hue.png"_spr);
				break;
			}
			case 2923: {
				setIcon("color.png"_spr);
				break;
			}
			case 2924: {
				setIcon("screen.png"_spr);
				break;
			}		
			
		}
		
		if (defalt == true)
		switch(m_objectID) {
			case 901: {
				setIcon("move.png"_spr);
				break;
			}
			case 1006: {
				setIcon("pulse.png"_spr);
				break;
			}
			case 1007: {
				setIcon("alpha.png"_spr);
				break;
			}
			case 1346: {
				setIcon("rotate.png"_spr);
				break;
			}
			case 2067: {
				setIcon("scale.png"_spr);
				break;
			}
			case 1585: {
				setIcon("animate.png"_spr);
				break;
			}
			case 3016: {
				setIcon("advfollow.png"_spr);
				break;
			}
			case 3660: {
				setIcon("editadv.png"_spr);
				break;
			}
			case 3661: {
				setIcon("target.png"_spr);
				break;
			}
			case 1814: {
				setIcon("followy.png"_spr);
				break;
			}
			case 1935: {
				setIcon("timewarp.png"_spr);
				break;
			}
			case 1932: {
				setIcon("control.png"_spr);
				break;
			}
			case 2999: {
				setIcon("mg.png"_spr);
				break;
			}
			case 3606: {
				setIcon("bgs.png"_spr);
				break;
			}
			case 3612: {
				setIcon("mgs.png"_spr);
				break;
			}
			case 3613: {
				setIcon("ui.png"_spr);
				break;
			}
			case 2899: {
				setIcon("options.png"_spr);
				break;
			}
			case 3602: {
				setIcon("sfx.png"_spr);
				break;
			}
			case 3603: {
				setIcon("esfx.png"_spr);
				break;
			}
			case 31: {
				if (!startp) {
					setIcon("start.png"_spr);
					break;
				}
				else {
					break;
				}	
			}
			case 3600: {
				setIcon("end.png"_spr);
				break;
			}
			case 2901: {
				setIcon("gpoff.png"_spr);
				break;
			}
			case 1917: {
				setIcon("reverse.png"_spr);
				break;
			}
			case 1934: {
				setIcon("song.png"_spr);
				break;
			}
			case 3605: {
				setIcon("editsong.png"_spr);
				break;
			} 
		}
		
		if (logic == true)
		switch(m_objectID) {
			case 1616: {
				setIcon("stop.png"_spr);
				break;
			}
			case 1817: {
				setIcon("pickup.png"_spr);
				break;
			}
			case 1268: {
				setIcon("spawn.png"_spr);
				break;
			}
			case 1347: {
				setIcon("follow.png"_spr);
				break;
			}
			case 1912: {
				setIcon("random.png"_spr);
				break;
			}
			case 2068: {
				setIcon("advrandom.png"_spr);
				break;
			}
			case 1611: {
				setIcon("count.png"_spr);
				break;
			}
			case 1811: {
				setIcon("advcount.png"_spr);
				break;
			}
			case 1595: {
				setIcon("touch.png"_spr);
				break;
			}
			case 3619: {
				setIcon("edit.png"_spr);
				break;
			}
			case 3620: {
				setIcon("comp.png"_spr);
				break;
			}
			case 3641: {
				setIcon("pers.png"_spr);
				break;
			}
			case 1812: {
				setIcon("dead.png"_spr);
				break;
			}
			case 1815: {
				setIcon("colis.png"_spr);
				break;
			}
			case 3609: {
				setIcon("advcolis.png"_spr);
				break;
			}
			case 3604: {
				setIcon("ev.png"_spr);
				break;
			}
		}

		if (area == true)
		switch(m_objectID) {
			case 3006: {
				setIcon("amove.png"_spr);
				break;
			}
			case 3007: {
				setIcon("arotate.png"_spr);
				break;
			}
			case 3008: {
				setIcon("ascale.png"_spr);
				break;
			}
			case 3009: {
				setIcon("aalpha.png"_spr);
				break;
			}
			case 3010: {
				setIcon("apulse.png"_spr);
				break;
			}
			case 3011: {
				setIcon("eamove.png"_spr);
				break;
			}
			case 3012: {
				setIcon("earotate.png"_spr);
				break;
			}
			case 3013: {
				setIcon("eascale.png"_spr);
				break;
			}
			case 3014: {
				setIcon("eaalpha.png"_spr);
				break;
			}
			case 3015: {
				setIcon("eapulse.png"_spr);
				break;
			}
			case 3024: {
				if (cstop) {
					setIcon("astopc.png"_spr);
					break;
				}
				else {
					setIcon("astop.png"_spr);
					break;
				}
			}
			case 3017: {
				setIcon("emove.png"_spr);
				break;
			}
			case 3018: {
				setIcon("erotate.png"_spr);
				break;
			}
			case 3019: {
				setIcon("escale.png"_spr);
				break;
			}
			case 3020: {
				setIcon("ealpha.png"_spr);
				break;
			}
			case 3021: {
				setIcon("epulse.png"_spr);
				break;
			}
			case 3023: {
				if (cstop) {
					setIcon("estopc.png"_spr);
					break;
				}
				else {
					setIcon("estop.png"_spr);
					break;
				}
			}
		}

		if (cam == true) {
			if (ccolor == false) {
				switch(m_objectID) {
					case 1913: {
						setIcon("czoom.png"_spr);
							break;
					}
					case 1914: {
						setIcon("cstatic.png"_spr);
							break;
					}
					case 1916: {
						setIcon("Coffset.png"_spr);
							break;
					}
					case 2015: {
						setIcon("crotate.png"_spr);
							break;
					}
					case 2062: {
						setIcon("edge.png"_spr);
							break;
					}
					case 2925: {
						setIcon("mode.png"_spr);
							break;
					}
					case 2016: {
						setIcon("guide.png"_spr);
							break;
					}
					case 1520: {
						if (!shakep) {
							setIcon("shake.png"_spr);
							break;
						}
						else {
							break;
						}
					}		
				}
			}
			else {
				switch(m_objectID) {
					case 1913: {
						setIcon("zoom.png"_spr);
							break;
					}
					case 1914: {
						setIcon("static.png"_spr);
							break;
					}
					case 1916: {
						setIcon("offset.png"_spr);
							break;
					}
					case 2015: {
						setIcon("rotatecam.png"_spr);
							break;
					}
					case 2062: {
						setIcon("edge.png"_spr);
							break;
					}
					case 2925: {
						setIcon("mode.png"_spr);
							break;
					}
					case 2016: {
						setIcon("guide.png"_spr);
							break;
					}
					case 1520: {
						if (!shakep) {
							setIcon("shake.png"_spr);
							break;
						}
						else {
							break;
						}
					}
				}
			}	
		}
		
		if (colis == true) 
		switch(m_objectID) {
			case 3640: {
				setIcon("colisin.png"_spr);
				break;
			}
			case 1816: {
				setIcon("colisblock.png"_spr);
				break;
			}
			case 3643: {
				setIcon("colistouch.png"_spr);
				break;
			}
		}
		
	}	

	void setIcon(std::string texture){
		if (CCSprite* newSpr = CCSprite::create(texture.c_str())) {
			m_addToNodeContainer = true;
			setTexture(newSpr->getTexture());
			setTextureRect(newSpr->getTextureRect());
		}
	}

	void setCombinedIcon(MyEffectGameObject* obj, const char* tex1, const char* tex2) {
		if (!obj) return;

		auto spr1 = CCSprite::create(tex1);
		auto spr2 = CCSprite::create(tex2);
		if (!spr1 || !spr2) return;

		auto size1 = spr1->getContentSize();
		auto size2 = spr2->getContentSize();

		float width  = std::max(size1.width,  size2.width);
		float height = std::max(size1.height, size2.height);

		auto rt = CCRenderTexture::create(width, height);
		if (!rt) return;

		spr1->setAnchorPoint({0.f, 0.f});
		spr2->setAnchorPoint({0.f, 0.f});
		spr1->setPosition({0.f, 0.f});
		spr2->setPosition({0.f, 0.f});

		rt->beginWithClear(0, 0, 0, 0);
		spr1->visit();
		spr2->visit();
		rt->end();

		rt->getSprite()->setFlipY(true);

		auto result = CCSprite::createWithTexture(rt->getSprite()->getTexture());
		if (!result) return;

		obj->m_addToNodeContainer = true;
		obj->setTexture(result->getTexture());
		obj->setTextureRect({0, 0, width, height});
	}
};
