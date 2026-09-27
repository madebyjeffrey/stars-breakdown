#include "core.h"

/* NE 0A:1086-12B2. Lay out and paint a non-client tile while preserving the
 * title-bar/button branch controlled by the packed tile flags. */
StarsI16 stars_draw_tile_nc(StarsDrawTileNCInput *input)
{
    StarsRect title_rect;
    StarsRect button_rect;
    StarsU16 button_flags;

    if (!input || !input->rect)
        return 0;
    input->rect->left = (StarsI16)((input->tile_flags & 7U) * 0xc6U + 4U);
    input->rect->right = (StarsI16)(input->rect->left + 0xbe);
    input->rect->top = input->y_top;
    input->rect->bottom = (StarsI16)(input->rect->top +
        ((input->tile_flags & 0x0080U) ? input->dy_full :
        (StarsI16)(input->arial8_height + 3)));
    if ((input->tile_flags & 0x1000U) == 0U ||
            (input->tile_flags & 0x0200U) != 0U) {
        if ((input->tile_flags & 0x1000U) == 0U && input->draw_frame)
            input->draw_frame(input->context, input->hdc, input->rect, 0U);
        title_rect = *input->rect;
        title_rect.left = (StarsI16)(title_rect.left + 1);
        title_rect.right = (StarsI16)(title_rect.right - 1);
        title_rect.top = (StarsI16)(title_rect.top + 1);
        title_rect.bottom = (StarsI16)(title_rect.bottom - 1);
        title_rect.bottom = (StarsI16)(title_rect.top +
            input->arial8_height + 2);
        if (input->select_object)
            input->select_object(input->context, input->hdc,
                input->font_arial8);
        if (input->set_text_color)
            input->set_text_color(input->context, input->hdc,
                input->text_color);
        if (input->set_background_color)
            input->set_background_color(input->context, input->hdc,
                input->face_color);
        if ((input->tile_flags & 0x1000U) == 0U && input->draw_frame)
            input->draw_frame(input->context, input->hdc, &title_rect, 0U);
        if (input->center_title)
            input->center_title(input->context, input->hdc, &title_rect,
                input->title);
        button_rect.left = (StarsI16)(input->rect->right - 0x11);
        button_rect.top = (StarsI16)(input->rect->top + 1);
        button_rect.right = input->rect->right;
        button_rect.bottom = (StarsI16)(title_rect.bottom + 1);
        button_flags = 0x70U;
        if ((input->tile_flags & 0x0080U) == 0U)
            button_flags = (StarsU16)(button_flags | 1U);
        if (input->draw_button)
            input->draw_button(input->context, input->hdc, &button_rect,
                button_flags);
        if (input->select_object)
            input->select_object(input->context, input->hdc,
                input->button_shadow_brush);
        if (input->pat_blt)
            input->pat_blt(input->context, input->hdc,
                (StarsI16)(input->rect->right - 0x12),
                (StarsI16)(input->rect->top + 1), 1,
                (StarsI16)(title_rect.bottom - input->rect->top - 1),
                0xf00021UL);
    }
    input->rect->top = (StarsI16)(input->rect->top +
        input->arial8_height + 4);
    return (input->tile_flags & 0x0080U) != 0U;
}
