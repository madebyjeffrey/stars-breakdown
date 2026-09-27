#include "core.h"

/* Genuine C reconstruction of ScrollScanner, NE 0C:6D30-6F30. */
void stars_scroll_scanner(StarsI16 dx, StarsI16 dy,
    StarsScrollScannerInput *input)
{
    StarsRect client, vertical, exposed;
    StarsU16 dc;
    StarsI16 ax, ay;
    if (!input || !input->ops || (dx == 0 && dy == 0) ||
            input->scanner_window == 0U)
        return;
    if (input->ops->visible &&
            !input->ops->visible(input->context, input->scanner_window)) return;
    if ((input->view_flags & 1U) != 0U) return;
    dc = input->ops->get_dc ? input->ops->get_dc(input->context,
        input->scanner_window) : 0;
    client.left = client.top = client.right = client.bottom = 0;
    if (input->ops->get_client_rect)
        input->ops->get_client_rect(input->context, input->scanner_window,
            &client);
    ax = dx < 0 ? (StarsI16)-dx : dx;
    ay = dy < 0 ? (StarsI16)-dy : dy;
    if ((StarsI16)(client.right / 2) < ax ||
            (StarsI16)(client.bottom / 2) < ay || input->dialog_up != 0 ||
            input->browser_window != 0U) {
        if (input->ops->draw)
            input->ops->draw(input->context, dc, input->scanner_window,
                &client);
    } else {
        client.bottom = (StarsU16)((StarsI16)client.bottom - input->dy_sbar);
        if (input->ops->update)
            input->ops->update(input->context, input->scanner_window);
        if (input->ops->scroll_window)
            input->ops->scroll_window(input->context, input->scanner_window,
                dx, dy, &client, &client);
        if (dy < 0) {
            vertical.left = 0;
            vertical.top = (StarsU16)((StarsI16)client.bottom + dy);
            vertical.right = client.right; vertical.bottom = client.bottom;
            client.bottom = (StarsU16)((StarsI16)client.bottom + dy);
        } else if (dy > 0) {
            vertical.left = 0; vertical.top = 0; vertical.right = client.right;
            vertical.bottom = (StarsU16)dy;
            client.top = (StarsU16)((StarsI16)client.top + dy);
        }
        if (dx < 0) {
            exposed.top = client.top; exposed.right = client.right;
            exposed.bottom = client.bottom;
            exposed.left = (StarsU16)((StarsI16)client.right + dx);
            client.left = (StarsU16)((StarsI16)client.left - dx);
        } else if (dx > 0) {
            exposed.left = 0; exposed.top = client.top;
            exposed.right = (StarsU16)dx; exposed.bottom = client.bottom;
            client.right = (StarsU16)((StarsI16)client.right - dx);
        }
        if (dx != 0 && input->ops->validate_rect)
            input->ops->validate_rect(input->context, input->scanner_window,
                &exposed);
        if (dy != 0 && input->ops->validate_rect)
            input->ops->validate_rect(input->context, input->scanner_window,
                &vertical);
        if (dx != 0 && input->ops->draw)
            input->ops->draw(input->context, dc, input->scanner_window,
                &exposed);
        if (dy != 0 && input->ops->draw)
            input->ops->draw(input->context, dc, input->scanner_window,
                &vertical);
        if (input->ops->update)
            input->ops->update(input->context, input->scanner_window);
    }
    if (input->ops->release_dc)
        input->ops->release_dc(input->context, input->scanner_window, dc);
}
