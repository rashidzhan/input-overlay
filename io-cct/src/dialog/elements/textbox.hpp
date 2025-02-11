/**
 * Created by universal on 8.1.17
 * This file is part of reloded which is licensed
 * under the MOZILLA PUBLIC LICENSE 2.0 - http://www.gnu.org/licenses
 * github.com/univrsal/input-overlay
 */

#pragma once

#include "gui_element.hpp"
#include "../dialog.hpp"
#include <string>

class dialog;

class gui_element;

class textbox : public gui_element {
public:
	textbox(int8_t id, int x, int y, int w, int h, std::string text, dialog *parent);

	~textbox();

	void close() override;

	void draw_background() override;

	void draw_foreground() override;

	bool handle_events(SDL_Event *event, bool was_handled) override;

	bool can_select() override;

	void select_state(bool state) override;

	uint8_t get_cursor() override;

	void set_text(std::string s);

	void set_hex_int(uint16_t i);

	void append_text(const std::string &s);

	const std::string *get_text();

	const char *c_str();

	void set_alert(bool state);

	void set_cutoff(uint8_t c);

private:
	static inline bool is_numeric(const std::string &s);

	static inline bool is_alpha_numeric(const std::string &s);

	static inline bool is_space_free(const std::string &s);

	static inline bool is_hex(const std::string &s);

	/*
	    Properly deletes last character in
	    unicode utf8 strings
	*/
	static void pop_back(std::string &s);

	std::string m_text;
	std::string m_composition;
	std::string m_cut_text;
	uint8_t m_cut_off = 38;
	bool m_alert = false;
};
