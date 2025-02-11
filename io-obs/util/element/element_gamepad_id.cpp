/**
 * This file is part of input-overlay
 * which is licensed under the GPL v2.0
 * See LICENSE or http://www.gnu.org/licenses
 * github.com/univrsal/input-overlay
 */

#include "../../sources/input_source.hpp"
#include "../../../ccl/ccl.hpp"
#include "element_gamepad_id.hpp"
#include "util/layout_constants.hpp"
#include "element_button.hpp"

element_gamepad_id::element_gamepad_id() : element_texture(ET_GAMEPAD_ID), m_mappings{}
{
    m_keycode = VC_PAD_GUIDE;
}

void element_gamepad_id::load(ccl_config* cfg, const std::string &id)
{
    element_texture::load(cfg, id);
    auto i = 1;
    for (auto &map : m_mappings) {
        map = m_mapping;
        map.x += i++ * (m_mapping.cx + CFG_INNER_BORDER);
    }
}

void element_gamepad_id::draw(gs_effect_t* effect, gs_image_file_t* image, element_data* data,
                              sources::overlay_settings* settings)
{
    if (data) {
        const auto d = dynamic_cast<element_data_button*>(data);
        if (d && (int) d->get_state()) {
            element_texture::draw(effect, image, &m_mappings[ID_PRESSED]);
        }
    }

    if (settings->gamepad > 0) {
        element_texture::draw(effect, image, &m_mappings[settings->gamepad - 1]);
    } else {
        element_texture::draw(effect, image, &m_mapping);
    }
}

data_source element_gamepad_id::get_source()
{
    return DS_GAMEPAD;
}
