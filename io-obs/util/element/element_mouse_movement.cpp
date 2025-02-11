/**
 * This file is part of input-overlay
 * which is licensed under the GPL v2.0
 * See LICENSE or http://www.gnu.org/licenses
 * github.com/univrsal/input-overlay
 */

#include "../../sources/input_source.hpp"
#include "../../../ccl/ccl.hpp"
#include "element_mouse_movement.hpp"
#include "util/layout_constants.hpp"
#include "util/util.hpp"

void element_mouse_movement::load(ccl_config* cfg, const std::string &id)
{
    element_texture::load(cfg, id);
    m_keycode = VC_MOUSE_DATA;
    m_radius = cfg->get_int(id + CFG_MOUSE_RADIUS);
    m_movement_type = cfg->get_int(id + CFG_MOUSE_TYPE) == 0 ? MM_DOT : MM_ARROW;
}

void element_mouse_movement::draw(gs_effect_t* effect, gs_image_file_t* image, element_data* data,
                                  sources::overlay_settings* settings)
{
    if (data && data->get_type() == ET_MOUSE_STATS) {
        const auto element_data = dynamic_cast<element_data_mouse_pos*>(data);

        if (element_data) {
            if (m_movement_type == MM_ARROW) {
                element_texture::draw(effect, image, &m_mapping, &m_pos, element_data->get_mouse_angle(settings));
            } else {
                element_data->get_mouse_offset(settings, m_pos, m_offset_pos, m_radius);
                element_texture::draw(effect, image, &m_mapping, &m_offset_pos);
            }
        }
    } else {
        element_texture::draw(effect, image, &m_mapping, &m_pos);
    }
}

element_data_mouse_pos::element_data_mouse_pos(const int16_t x, const int16_t y) :
    element_data(ET_MOUSE_STATS)
{
    m_x = x;
    m_y = y;
}

bool element_data_mouse_pos::is_persistent()
{
    return true;
}

bool element_data_mouse_pos::merge(element_data *other)
{
    if (other) {
        element_data_mouse_pos* pos = dynamic_cast<element_data_mouse_pos*>(other);
        if (pos) {
            m_last_x = m_x;
            m_last_y = m_x;
            m_x = pos->m_x;
            m_y = pos->m_y;
        }
    }
    return false;
}

float element_data_mouse_pos::get_mouse_angle(sources::overlay_settings* settings)
{
    auto d_x = 0, d_y = 0;

    if (settings->use_center) {
        d_x = m_x - settings->monitor_h;
        d_y = m_y - settings->monitor_w;
    } else {
        d_x = m_x - m_last_x;
        d_y = m_y - m_last_y;
    }

    const float new_angle = (0.5 * M_PI) + (atan2f(d_y, d_x));
    if (abs(d_x) < settings->mouse_deadzone || abs(d_y) < settings->mouse_deadzone) {
        /* Draw old angle (new movement was to minor) */
        return m_old_angle;
    }

    m_old_angle = new_angle;
    return new_angle;
}

void element_data_mouse_pos::get_mouse_offset(sources::overlay_settings* settings, const vec2 &center, vec2 &out,
                                                const uint8_t radius) const
{
    auto d_x = 0, d_y = 0;

    if (settings->use_center) {
        d_x = m_x - settings->monitor_h;
        d_y = m_y - settings->monitor_w;
    } else {
        d_x = m_x - m_last_x;
        d_y = m_y - m_last_y;

        if (abs(d_x) < settings->mouse_deadzone)
            d_x = 0;
        if (abs(d_y) < settings->mouse_deadzone)
            d_y = 0;
    }

    const auto factor_x = UTIL_CLAMP(-1, ((double) d_x / settings->mouse_sens), 1);
    const auto factor_y = UTIL_CLAMP(-1, ((double) d_y / settings->mouse_sens), 1);

    out.x = center.x + radius * factor_x;
    out.y = center.y + radius * factor_y;

}

int16_t element_data_mouse_pos::get_mouse_x() const
{
    return m_x;
}

int16_t element_data_mouse_pos::get_mouse_y() const
{
    return m_y;
}

element_mouse_movement::element_mouse_movement() :
    element_texture(ET_MOUSE_STATS), m_movement_type()
{
};
