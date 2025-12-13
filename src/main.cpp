#include <Geode/Geode.hpp>
#include <Geode/modify/EffectGameObject.hpp>
#include <Geode/modify/CCSpriteBatchNode.hpp>
#include <Geode/modify/LevelEditorLayer.hpp>
#include <Geode/modify/CCSprite.hpp>

using namespace geode::prelude;

class $modify(MyEffectGameObject, EffectGameObject) {
	void customSetup() {
		EffectGameObject::customSetup();

		auto defalt = Mod::get()->getSettingValue<bool>("do-default");
		auto logic = Mod::get()->getSettingValue<bool>("do-logic");
		auto shader = Mod::get()->getSettingValue<bool>("do-shader");
		auto area = Mod::get()->getSettingValue<bool>("do-area");
		auto ccolor = Mod::get()->getSettingValue<bool>("color-cam");
		auto cam = Mod::get()->getSettingValue<bool>("do-cam");

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
				setIcon("astop.png"_spr);
				break;
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
				setIcon("estop.png"_spr);
				break;
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
						setIcon("coffset.png"_spr);
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
				}
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
