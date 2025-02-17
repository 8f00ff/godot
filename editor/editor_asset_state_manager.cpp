/**************************************************************************/
/*  editor_asset_installer.h                                              */
/**************************************************************************/
/*                         This file is part of:                          */
/*                             GODOT ENGINE                               */
/*                        https://godotengine.org                         */
/**************************************************************************/
/* Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md). */
/* Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.                  */
/*                                                                        */
/* Permission is hereby granted, free of charge, to any person obtaining  */
/* a copy of this software and associated documentation files (the        */
/* "Software"), to deal in the Software without restriction, including    */
/* without limitation the rights to use, copy, modify, merge, publish,    */
/* distribute, sublicense, and/or sell copies of the Software, and to     */
/* permit persons to whom the Software is furnished to do so, subject to  */
/* the following conditions:                                              */
/*                                                                        */
/* The above copyright notice and this permission notice shall be         */
/* included in all copies or substantial portions of the Software.        */
/*                                                                        */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,        */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF     */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. */
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY   */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,   */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE      */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                 */
/**************************************************************************/

#include "editor_asset_state_manager.h"

#include "core/config/project_settings.h"
#include "core/io/config_file.h"
#include "editor/editor_file_system.h"

EditorAssetStateManager *EditorAssetStateManager::singleton = nullptr;

const char *EditorAssetStateManager::ADDONS_LOCK_PATH = "res://addons_lock.cfg";

void EditorAssetStateManager::_bind_methods() {}

EditorAssetStateManager::AssetState* EditorAssetStateManager::get_asset_state(int p_asset_id) {
	load_lock_file();

	AssetState* state = asset_states.getptr(p_asset_id);
	if (!state) {
		return nullptr;
	}
	return state;
}

void EditorAssetStateManager::register_installed_asset(int p_asset_id, const String &p_asset_name, const int &p_asset_version, const String &p_install_folder, const bool &p_skip_toplevel, const Vector<String> &p_file_paths) {
	AssetState state;
	state.asset_name = p_asset_name;
	state.asset_version = p_asset_version;
	state.install_folder = p_install_folder;
	state.skip_toplevel = p_skip_toplevel;
	state.file_paths = p_file_paths;
	asset_states[p_asset_id] = state;

	save_lock_file();
}

void EditorAssetStateManager::save_lock_file() {
	Ref<ConfigFile> config;
	config.instantiate();

	for (const KeyValue<int, AssetState>& E : asset_states) {
		String section;
		int asset_id = E.key;
		AssetState state = E.value;

		if (asset_id > 0) {
			section = itos(asset_id);
		} else {
			section = state.asset_name.get_file().get_basename();
		}

		config->set_value(section, "asset_name", state.asset_name);

		if (state.asset_version > 0) {
			config->set_value(section, "asset_version", state.asset_version);
		}

		config->set_value(section, "install_folder", state.install_folder);
		config->set_value(section, "skip_toplevel", state.skip_toplevel);
		config->set_value(section, "file_paths", state.file_paths);
	}

	config->save(ADDONS_LOCK_PATH);
}

void EditorAssetStateManager::load_lock_file() {
	Ref<ConfigFile> config;
	config.instantiate();

	Error err = config->load(ADDONS_LOCK_PATH);
	if (err != OK) {
		return;
	}

	List<String> sections;
	config->get_sections(&sections);

	for (const String &section : sections) {
		AssetState state;

		if (!config->has_section_key(section, "asset_name") || !config->has_section_key(section, "file_paths")) {
			WARN_PRINT(vformat("Invalid asset state in lock file."));
			continue;
		}

		int asset_id = 0;
		if (section.is_valid_int()) {
			asset_id = section.to_int();
		}

		state.asset_name = config->get_value(section, "asset_name");
		state.asset_version = config->get_value(section, "asset_version", 0);
		state.install_folder = config->get_value(section, "install_folder", "res://");
		state.skip_toplevel = config->get_value(section, "skip_toplevel", true);
		state.file_paths = config->get_value(section, "file_paths");
		asset_states[asset_id] = state;
	}
}

EditorAssetStateManager::EditorAssetStateManager() {
	singleton = this;

	load_lock_file();
}

EditorAssetStateManager::~EditorAssetStateManager() {
	singleton = nullptr;
}
