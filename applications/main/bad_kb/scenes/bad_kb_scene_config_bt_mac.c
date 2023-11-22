#include "../bad_kb_app.h"

typedef struct {
    char mac_address[BAD_KB_MAC_LEN];
    char nickname[32];
} MacNickname;

#define MAX_MAC_NICKNAMES 10

typedef struct {
    MacNickname nicknames[MAX_MAC_NICKNAMES];
    int num_nicknames;
} MacNicknameMenu;

MacNicknameMenu mac_nickname_menu;

void bad_kb_scene_config_bt_mac_byte_input_callback(void* context) {
    BadKbApp* bad_kb = context;

    view_dispatcher_send_custom_event(bad_kb->view_dispatcher, BadKbAppCustomEventByteInputDone);
}

void bad_kb_scene_config_bt_mac_on_enter(void* context) {
    BadKbApp* bad_kb = context;
    ByteInput* byte_input = bad_kb->byte_input;

    memmove(bad_kb->bt_mac_buf, bad_kb->config.bt_mac, BAD_KB_MAC_LEN);
    furi_hal_bt_reverse_mac_addr(bad_kb->bt_mac_buf);
    byte_input_set_header_text(byte_input, "Set BT MAC address");

    byte_input_set_result_callback(
        byte_input,
        bad_kb_scene_config_bt_mac_byte_input_callback,
        NULL,
        bad_kb,
        bad_kb->bt_mac_buf,
        BAD_KB_MAC_LEN);

    view_dispatcher_switch_to_view(bad_kb->view_dispatcher, BadKbAppViewByteInput);
}

bool bad_kb_scene_config_bt_mac_on_event(void* context, SceneManagerEvent event) {
    BadKbApp* bad_kb = context;
    bool consumed = false;

    if(event.type == SceneManagerEventTypeCustom) {
        consumed = true;
        if(event.event == BadKbAppCustomEventByteInputDone) {
            furi_hal_bt_reverse_mac_addr(bad_kb->bt_mac_buf);
            memmove(bad_kb->config.bt_mac, bad_kb->bt_mac_buf, BAD_KB_MAC_LEN);
            bad_kb_config_refresh(bad_kb);
        }
        scene_manager_previous_scene(bad_kb->scene_manager);
    }
    return consumed;
}

void bad_kb_scene_config_bt_mac_on_exit(void* context) {
    BadKbApp* bad_kb = context;
    ByteInput* byte_input = bad_kb->byte_input;

    byte_input_set_result_callback(byte_input, NULL, NULL, NULL, NULL, 0);
    byte_input_set_header_text(byte_input, "");
}

void bad_kb_scene_config_bt_mac_submenu_create_nickname(void* context) {
    BadKbApp* bad_kb = context;
    MacNicknameMenu* menu = &mac_nickname_menu;

    if (menu->num_nicknames >= MAX_MAC_NICKNAMES) {
        // Maximum number of nicknames reached
        return;
    }

    // Get the current MAC address
    char mac_address[BAD_KB_MAC_LEN];
    memmove(mac_address, bad_kb->config.bt_mac, BAD_KB_MAC_LEN);
    furi_hal_bt_reverse_mac_addr(mac_address);

    // Prompt the user for a nickname
    char nickname[32];
    // Code to prompt the user for a nickname and store it in the 'nickname' variable

    // Add the MAC address and nickname to the menu
    memmove(menu->nicknames[menu->num_nicknames].mac_address, mac_address, BAD_KB_MAC_LEN);
    memmove(menu->nicknames[menu->num_nicknames].nickname, nickname, strlen(nickname));
    menu->num_nicknames++;
}

void bad_kb_scene_config_bt_mac_submenu_switch_nickname(void* context) {
    BadKbApp* bad_kb = context;
    MacNicknameMenu* menu = &mac_nickname_menu;

    // Code to display the list of nicknames and prompt the user to select one

    // Get the selected nickname
    int selected_index = 0; // Index of the selected nickname in the menu
    char selected_mac_address[BAD_KB_MAC_LEN];
    memmove(selected_mac_address, menu->nicknames[selected_index].mac_address, BAD_KB_MAC_LEN);

    // Update the BT MAC address in the configuration
    memmove(bad_kb->config.bt_mac, selected_mac_address, BAD_KB_MAC_LEN);
    bad_kb_config_refresh(bad_kb);
}
