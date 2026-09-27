#include <windows.h>
#include <toolhelp.h>
#include <stdio.h>

/* Read only the 22-byte GlobalPD record from the original STARS module.
 * CodeView resolves it to NE segment 37, offset 0x0b80. This helper never
 * writes to the original module; the caller triggers report clicks. */
#define STARS_DATA_SEGMENT 37U
#define GLOBALPD_OFFSET 0x0b80UL
#define GLOBALPD_SIZE 22U
#define POPUP_HANDLE_OFFSET 0x0b7eUL
#define PLAYER_ID_OFFSET 0x018cUL
#define SHDEF_TABLE_OFFSET 0x014cUL
#define SELECTED_PLANET_STARBASE_OFFSET (0x4956UL + 44UL)

int PASCAL WinMain(HINSTANCE instance, HINSTANCE previous,
    LPSTR command_line, int show)
{
    MODULEENTRY module;
    GLOBALENTRY global;
    HMODULE handle;
    WORD selector;
    DWORD read_count;
    unsigned char bytes[GLOBALPD_SIZE];
    FILE *report;
    FILE *binary;
    unsigned int index;
    HWND report_window;
    HWND popup_window;
    unsigned char popup_handle[2];
    unsigned char player_word[2];
    unsigned char shdef_pointer[4];
    unsigned char starbase_word[2];
    unsigned int player_id;
    unsigned int shdef_base;
    unsigned int design_index;
    int click_x;
    int click_y;
    int click_report;
    char local_command[32];
    unsigned int command_length;
    RECT popup_rect;
    RECT popup_client;
    RECT report_rect;
    POINT screen_point;
    char popup_class[64];

    (void)instance;
    (void)previous;
    (void)show;
    command_length = 0U;
    if (command_line)
        while (command_line[command_length] &&
                command_length + 1U < sizeof local_command) {
            local_command[command_length] = command_line[command_length];
            ++command_length;
        }
    local_command[command_length] = 0;
    click_report = sscanf(local_command, "%d %d", &click_x, &click_y) == 2;
    popup_window = 0;
    report = fopen("CAPTURE.TXT", "w");
    if (!report)
        return 2;
    module.dwSize = sizeof module;
    handle = ModuleFindName(&module, "STARS!");
    if (!handle) {
        fprintf(report, "STATUS=MODULE_NOT_FOUND\n");
        module.dwSize = sizeof module;
        if (ModuleFirst(&module)) {
            do {
                fprintf(report, "VISIBLE_MODULE=%s HANDLE=%04X\n",
                    module.szModule, (unsigned int)module.hModule);
                module.dwSize = sizeof module;
            } while (ModuleNext(&module));
        }
        fclose(report);
        return 1;
    }
    fprintf(report, "MODULE=%04X PATH=%s\n", (unsigned int)handle,
        module.szExePath);
    global.dwSize = sizeof global;
    if (!GlobalEntryModule(&global, handle, STARS_DATA_SEGMENT)) {
        fprintf(report, "STATUS=SEGMENT_NOT_FOUND\n");
        fclose(report);
        return 1;
    }
    selector = GlobalHandleToSel(global.hBlock);
    fprintf(report, "SEGMENT=%u BLOCK=%04X SELECTOR=%04X SIZE=%lu\n",
        STARS_DATA_SEGMENT, (unsigned int)global.hBlock,
        (unsigned int)selector, (unsigned long)global.dwBlockSize);
    if (!selector || global.dwBlockSize < GLOBALPD_OFFSET + GLOBALPD_SIZE) {
        fprintf(report, "STATUS=INVALID_SEGMENT\n");
        fclose(report);
        return 1;
    }
    if (click_report) {
        report_window = FindWindow("starsreport", (LPCSTR)0);
        if (!report_window) {
            fprintf(report, "STATUS=REPORT_NOT_FOUND\n");
            fclose(report);
            return 1;
        }
        fprintf(report, "REPORT_HWND=%04X\n",
            (unsigned int)report_window);
        fprintf(report, "CLICK_CLIENT=%d,%d\n", click_x, click_y);
        GetWindowRect(report_window, &report_rect);
        fprintf(report, "REPORT_RECT=%d,%d,%d,%d\n",
            report_rect.left, report_rect.top,
            report_rect.right, report_rect.bottom);
        screen_point.x = click_x;
        screen_point.y = click_y;
        ClientToScreen(report_window, &screen_point);
        fprintf(report, "CLICK_SCREEN=%d,%d\n",
            screen_point.x, screen_point.y);
        SendMessage(report_window, 0x0201U, 1U,
            MAKELONG((WORD)click_x, (WORD)click_y));
        if (MemoryRead(selector, POPUP_HANDLE_OFFSET, popup_handle, 2U)
                == 2UL) {
            popup_window = (HWND)((unsigned int)popup_handle[0] |
                ((unsigned int)popup_handle[1] << 8));
            fprintf(report, "POPUP_HWND=%04X\n",
                (unsigned int)popup_window);
            if (popup_window && IsWindow(popup_window)) {
                GetWindowRect(popup_window, &popup_rect);
                fprintf(report, "POPUP_RECT=%d,%d,%d,%d\n",
                    popup_rect.left, popup_rect.top,
                    popup_rect.right, popup_rect.bottom);
                GetClientRect(popup_window, &popup_client);
                fprintf(report, "POPUP_CLIENT=%d,%d,%d,%d\n",
                    popup_client.left, popup_client.top,
                    popup_client.right, popup_client.bottom);
                if (GetClassName(popup_window, popup_class,
                        sizeof popup_class))
                    fprintf(report, "POPUP_CLASS=%s\n", popup_class);
                fprintf(report, "POPUP_PARENT=%04X\n",
                    (unsigned int)GetParent(popup_window));
                fprintf(report, "POPUP_VISIBLE=%d\n",
                    IsWindowVisible(popup_window) != 0);
                fprintf(report, "POPUP_CAPTURE=%04X\n",
                    (unsigned int)GetCapture());
                fprintf(report, "POPUP_FOCUS=%04X\n",
                    (unsigned int)GetFocus());
                fprintf(report, "POPUP_STYLE=%08lX\n",
                    (unsigned long)GetWindowLong(popup_window, GWL_STYLE));
            }
        }
    }
    read_count = MemoryRead(selector, GLOBALPD_OFFSET, bytes,
        GLOBALPD_SIZE);
    fprintf(report, "READ=%lu\n", (unsigned long)read_count);
    if (read_count != GLOBALPD_SIZE) {
        fprintf(report, "STATUS=READ_FAILED\n");
        if (popup_window && IsWindow(popup_window))
            SendMessage(popup_window, 0x0202U, 0U, 0L);
        fclose(report);
        return 1;
    }
    if (MemoryRead(selector, PLAYER_ID_OFFSET, player_word, 2U) == 2UL) {
        player_id = (unsigned int)player_word[0] |
            ((unsigned int)player_word[1] << 8);
        fprintf(report, "PLAYER_ID=%u\n", player_id);
        if (player_id < 16U && MemoryRead(selector,
                SHDEF_TABLE_OFFSET + (DWORD)player_id * 4UL,
                shdef_pointer, 4U) == 4UL) {
            shdef_base = (unsigned int)shdef_pointer[0] |
                ((unsigned int)shdef_pointer[1] << 8);
            fprintf(report, "SHDEF_BASE=%02X%02X:%04X\n",
                (unsigned int)shdef_pointer[3],
                (unsigned int)shdef_pointer[2], shdef_base);
            if (MemoryRead(selector, SELECTED_PLANET_STARBASE_OFFSET,
                    starbase_word, 2U) == 2UL) {
                design_index = ((unsigned int)starbase_word[0] |
                    ((unsigned int)starbase_word[1] << 8)) & 15U;
                fprintf(report, "STARBASE_DESIGN_INDEX=%u\n",
                    design_index);
                fprintf(report, "EXPECTED_STARBASE_FAR=%02X%02X:%04X\n",
                    (unsigned int)shdef_pointer[3],
                    (unsigned int)shdef_pointer[2],
                    (unsigned int)((shdef_base + design_index * 0x93U)
                    & 0xffffU));
            }
        }
    }
    fprintf(report, "GLOBALPD_HEX=");
    for (index = 0; index < GLOBALPD_SIZE; ++index)
        fprintf(report, "%02X", (unsigned int)bytes[index]);
    fprintf(report, "\nSTATUS=PASS\n");
    if (popup_window && IsWindow(popup_window)) {
        SendMessage(popup_window, 0x0202U, 0U, 0L);
        fprintf(report, "POPUP_RELEASED=1\n");
    }
    fclose(report);
    binary = fopen("CAPTURE.BIN", "wb");
    if (!binary)
        return 2;
    if (fwrite(bytes, 1, GLOBALPD_SIZE, binary) != GLOBALPD_SIZE) {
        fclose(binary);
        return 2;
    }
    fclose(binary);
    return 0;
}
