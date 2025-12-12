#include <Geode/Geode.hpp>
#include <Geode/modify/EffectGameObject.hpp>
#include <Geode/modify/CCSpriteBatchNode.hpp>
#include <Geode/modify/LevelEditorLayer.hpp>
#include <Geode/modify/CCSprite.hpp>

using namespace geode::prelude;

class $modify(MyEffectGameObject, EffectGameObject) {
	void customSetup() {
		EffectGameObject::customSetup();
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
	}

	void setIcon(std::string texture){
		if (CCSprite* newSpr = CCSprite::create(texture.c_str())) {
			m_addToNodeContainer = true;
			setTexture(newSpr->getTexture());
			setTextureRect(newSpr->getTextureRect());
		}
	}
};