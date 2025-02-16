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

#ifndef EDITOR_ASSET_STATE_MANAGER_H
#define EDITOR_ASSET_STATE_MANAGER_H

#include "core/object/object.h"
#include "core/io/resource_uid.h"

class EditorAssetStateManager : public Object {
	GDCLASS(EditorAssetStateManager, Object);

	static const char *ADDONS_LOCK_PATH;

	static EditorAssetStateManager *singleton;

protected:
	static void _bind_methods();

private:
	struct AssetState {
		int asset_id;
		String asset_name;
		int asset_version;
		String install_folder;
		bool skip_toplevel;
		Vector<String> file_paths;
	};

	Vector<AssetState> asset_states;

	AssetState* get_asset_state(int p_asset_id);

public:
	static EditorAssetStateManager *get_singleton() { return singleton; }

	void register_installed_asset(int p_asset_id, const String &p_asset_name, const int &p_asset_version, const String &p_install_folder, const bool &p_skip_toplevel, const Vector<String> &p_file_paths);
	void save_lock_file();
	void load_lock_file();

	EditorAssetStateManager();
	~EditorAssetStateManager();
};

#endif // EDITOR_ASSET_STATE_MANAGER_H
