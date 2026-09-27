#include "core.h"
#include <string.h>

/* Component information is a small report embedded in several dialogs.  The
 * original routine used a packed _part/_compart/_hul graph and selected GDI
 * objects directly.  These names document the stable report vocabulary while
 * the callbacks keep the 16-bit handles and resource tables at the boundary. */
enum {
    COMPONENT_STRING_NAME = 0x034e,
    COMPONENT_STRING_NO_PART = 0x034d,
    COMPONENT_STRING_NO_COST = 0x0352,
    COMPONENT_STRING_COST = 0x0351,
    COMPONENT_STRING_MASS = 0x0354,
    COMPONENT_STRING_ARMOR = 0x0355,
    COMPONENT_STRING_FUEL = 0x0358,
    COMPONENT_STRING_ENGINE = 0x0359,
    COMPONENT_STRING_POWER = 0x004b,
    COMPONENT_STRING_SHIELD = 0x035a,
    COMPONENT_STRING_SUMMARY = 0x0356,
    COMPONENT_STRING_DESCRIPTION = 0x0357,
    COMPONENT_STRING_HULL = 0x004a,
    COMPONENT_STRING_LEVEL = 0x004f,
    COMPONENT_STRING_TONNAGE = 0x004e,
    COMPONENT_STRING_RADAR = 0x0452,
    COMPONENT_STRING_RANGE = 0x0453,
    COMPONENT_STRING_PERCENT = 0x04b7,
    COMPONENT_STRING_NO_DATA = 0x0540,
    COMPONENT_FLAG_ARMOR_GRAPH = 0x0001,
    COMPONENT_FLAG_HULL_GRAPH = 0x0002,
    COMPONENT_FLAG_RADAR_GRAPH = 0x0004,
    COMPONENT_KIND_ARMOR = 0x0001,
    COMPONENT_KIND_ENGINE = 0x0002,
    COMPONENT_KIND_SHIELD = 0x0004,
    COMPONENT_KIND_WEAPON = 0x0008,
    COMPONENT_KIND_BOMB = 0x0010,
    COMPONENT_KIND_MINING = 0x0020,
    COMPONENT_KIND_SCANNER = 0x0040,
    COMPONENT_KIND_CLOAK = 0x0080,
    COMPONENT_KIND_SPECIAL = 0x0100,
    COMPONENT_KIND_BATTLE = 0x0200,
    COMPONENT_KIND_RADAR = 0x0400
};

static StarsI16 component_string(const StarsDisplayComponentInput *input,
    StarsU16 id, StarsU8 *buffer, StarsU16 capacity)
{
    if (!input->ops->get_string) return 0;
    return input->ops->get_string(input->context, id, buffer, capacity);
}

static StarsI16 component_number(const StarsDisplayComponentInput *input,
    StarsI32 value, StarsU8 *buffer, StarsU16 capacity)
{
    if (!input->ops->format_number) return 0;
    return input->ops->format_number(input->context, value, buffer, capacity);
}

static void component_text(const StarsDisplayComponentInput *input,
    StarsI16 x, StarsI16 y, const StarsU8 *text, StarsI16 length)
{
    if (input->ops->text_out)
        input->ops->text_out(input->context, input->hdc, x, y, text, length);
}

static void component_label(const StarsDisplayComponentInput *input,
    StarsI16 x, StarsI16 y, StarsU16 id)
{
    StarsU8 buffer[96];
    StarsI16 length;

    length = component_string(input, id, buffer, (StarsU16)sizeof buffer);
    component_text(input, x, y, buffer, length);
}

static void component_value(const StarsDisplayComponentInput *input,
    StarsI16 x, StarsI16 y, StarsI32 value)
{
    StarsU8 buffer[48];
    StarsI16 length;

    length = component_number(input, value, buffer, (StarsU16)sizeof buffer);
    component_text(input, x, y, buffer, length);
}

static void component_right_label(const StarsDisplayComponentInput *input,
    StarsI16 x, StarsI16 y, StarsU16 id)
{
    StarsU8 buffer[96];
    StarsI16 length;

    length = component_string(input, id, buffer, (StarsU16)sizeof buffer);
    if (input->ops->right_text)
        input->ops->right_text(input->context, input->hdc, x, y, buffer,
            length);
}

static void component_right_value(const StarsDisplayComponentInput *input,
    StarsI16 x, StarsI16 y, StarsI32 value)
{
    StarsU8 buffer[48];
    StarsI16 length;

    length = component_number(input, value, buffer, (StarsU16)sizeof buffer);
    if (input->ops->right_text)
        input->ops->right_text(input->context, input->hdc, x, y, buffer,
            length);
}

static void component_wrap_label(const StarsDisplayComponentInput *input,
    StarsRect *rect, StarsU16 id, StarsI16 color)
{
    StarsU8 buffer[160];
    StarsI16 length;

    length = component_string(input, id, buffer, (StarsU16)sizeof buffer);
    (void)length;
    if (input->ops->wrap_text)
        input->ops->wrap_text(input->context, input->hdc, rect, buffer,
            color);
}

static void component_draw_header(const StarsDisplayComponentInput *input,
    StarsRect *body)
{
    StarsU8 buffer[96];
    StarsI16 length;
    StarsI16 title_width;

    if (input->ops->select_font)
        input->ops->select_font(input->context, input->hdc,
            input->font_arial10);
    if (!input->part->name) {
        component_wrap_label(input, body, COMPONENT_STRING_NO_PART, 0);
        return;
    }
    length = (StarsI16)strlen((const char *)input->part->name);
    title_width = input->ops->text_extent ? input->ops->text_extent(
        input->context, input->hdc, input->part->name, length) : 0;
    if (input->ops->center_text)
        input->ops->center_text(input->context, input->hdc,
            (StarsI16)(input->width / 2), 3, input->part->name, length);
    if (input->part->image_id >= 0 && input->ops->draw_image)
        input->ops->draw_image(input->context, input->hdc, 5,
            (StarsI16)(input->arial10_height + 5), input->part->image_id);
    if (input->ops->select_font)
        input->ops->select_font(input->context, input->hdc,
            input->font_arial8);
    length = component_string(input, COMPONENT_STRING_NAME, buffer,
        (StarsU16)sizeof buffer);
    component_text(input, 5, (StarsI16)(input->arial10_height + 0x47),
        buffer, length);
    (void)title_width;
}

static StarsI32 component_cost(const StarsDisplayComponentInput *input)
{
    if (!input->ops->cost_of_developing) return -1L;
    return input->ops->cost_of_developing(input->context, input->player,
        input->part);
}

static void component_draw_costs(const StarsDisplayComponentInput *input,
    StarsRect *body, StarsI32 cost)
{
    static const StarsU16 labels[6] = { 0x005b, 0x005c, 0x005d, 0x005e,
        0x005f, 0x0060 };
    StarsI16 row;
    StarsI16 y;
    StarsI16 x;
    StarsU16 amount;
    StarsU32 color;

    x = (StarsI16)(body->left + input->max_mineral_width + 0x4d);
    y = (StarsI16)(input->arial10_height + 5);
    if (input->ops->select_font)
        input->ops->select_font(input->context, input->hdc,
            input->font_arial8_bold);
    for (row = 0; row < 6; ++row) {
        if (row == 3 || row == 4) continue;
        color = input->color_black;
        if (input->player_technology && input->player_technology[row] <
                input->part->mineral_cost[row])
            color = input->color_disabled;
        if (input->ops->set_text_color)
            input->ops->set_text_color(input->context, input->hdc, color);
        component_right_label(input, x, y, labels[row]);
        amount = input->part->mineral_cost[row];
        if ((input->part->kind == COMPONENT_KIND_ARMOR ||
                input->part->kind == COMPONENT_KIND_ENGINE) && amount > 0)
            amount = (StarsU16)(amount - amount / 2);
        component_right_value(input, (StarsI16)(x + input->max_mineral_width),
            y, amount);
        y = (StarsI16)(y + input->arial8_height);
    }
    if (input->ops->set_text_color)
        input->ops->set_text_color(input->context, input->hdc,
            input->color_black);
    if (input->part->mass != 0) {
        component_label(input, (StarsI16)(x + input->max_mineral_width + 0x20),
            input->arial10_height + 5, COMPONENT_STRING_MASS);
        component_value(input, (StarsI16)(x + input->max_mineral_width + 0x44),
            input->arial10_height + 5, input->part->mass);
    }
    if (cost < 0) {
        if (input->ops->set_text_color)
            input->ops->set_text_color(input->context, input->hdc,
                input->color_disabled);
        component_wrap_label(input, body, COMPONENT_STRING_NO_COST, 0);
    } else if (cost < 100000L) {
        component_label(input, 5, (StarsI16)(y + input->arial8_height + 4),
            COMPONENT_STRING_COST);
        component_value(input, 5 + input->max_mineral_width,
            (StarsI16)(y + input->arial8_height + 4), cost);
    } else {
        component_label(input, 5, (StarsI16)(y + input->arial8_height + 4),
            COMPONENT_STRING_COST);
        component_value(input, 5 + input->max_mineral_width,
            (StarsI16)(y + input->arial8_height + 4), (cost + 500L) / 1000L);
    }
}

static void component_draw_graph(const StarsDisplayComponentInput *input,
    StarsRect *body, StarsI16 graph_kind)
{
    if (input->ops->draw_graph)
        input->ops->draw_graph(input->context, input->hdc, body, graph_kind,
            input->part->armor_damage, input->part->resistance);
}

static void component_draw_detail(const StarsDisplayComponentInput *input,
    StarsRect *body)
{
    StarsI16 y;

    y = (StarsI16)(body->top + 4);
    if (input->ops->select_font)
        input->ops->select_font(input->context, input->hdc,
            input->font_arial8_bold);
    switch (input->part->kind) {
    case COMPONENT_KIND_ARMOR:
        component_wrap_label(input, body, COMPONENT_STRING_ARMOR, 0);
        component_right_label(input, body->right, y, COMPONENT_STRING_LEVEL);
        component_right_value(input, body->right, (StarsI16)(y +
            input->arial8_height), input->part->armor_damage);
        component_draw_graph(input, body, COMPONENT_FLAG_ARMOR_GRAPH);
        break;
    case COMPONENT_KIND_ENGINE:
        component_wrap_label(input, body, COMPONENT_STRING_ENGINE, 0);
        component_right_label(input, body->right, y, COMPONENT_STRING_FUEL);
        component_right_value(input, body->right, (StarsI16)(y +
            input->arial8_height), input->part->fuel_max);
        component_right_label(input, body->right, (StarsI16)(y +
            input->arial8_height * 2), COMPONENT_STRING_POWER);
        component_right_value(input, body->right, (StarsI16)(y +
            input->arial8_height * 3), input->part->engine_damage);
        break;
    case COMPONENT_KIND_SHIELD:
        component_wrap_label(input, body, COMPONENT_STRING_SHIELD, 0);
        component_draw_graph(input, body, COMPONENT_FLAG_HULL_GRAPH);
        break;
    case COMPONENT_KIND_RADAR:
    case COMPONENT_KIND_SCANNER:
        component_wrap_label(input, body, COMPONENT_STRING_RADAR, 0);
        component_right_label(input, body->right, y, COMPONENT_STRING_RANGE);
        component_right_value(input, body->right, (StarsI16)(y +
            input->arial8_height), input->part->resistance);
        component_draw_graph(input, body, COMPONENT_FLAG_RADAR_GRAPH);
        break;
    case COMPONENT_KIND_WEAPON:
    case COMPONENT_KIND_BOMB:
    case COMPONENT_KIND_MINING:
    case COMPONENT_KIND_CLOAK:
    case COMPONENT_KIND_SPECIAL:
    case COMPONENT_KIND_BATTLE:
    default:
        component_wrap_label(input, body, COMPONENT_STRING_DESCRIPTION, 0);
        if (input->part->component_flags & COMPONENT_FLAG_HULL_GRAPH)
            component_draw_graph(input, body, COMPONENT_FLAG_HULL_GRAPH);
        break;
    }
}

/* NE 28:2AC6-65AD.  Draw the component report: frame and title, component
 * image, mineral rows, development cost, and the category-specific detail
 * panel.  The packed original records are represented by StarsDisplay-
 * ComponentPart; callbacks own the original string, font, brush, and graph
 * services. */
void stars_display_component_info(const StarsDisplayComponentInput *input)
{
    StarsRect body;
    StarsI32 cost;

    if (!input || !input->part || !input->ops) return;
    body.left = 5;
    body.top = (StarsI16)(input->arial10_height + 0x47);
    body.right = (StarsI16)(input->width - 5);
    body.bottom = (StarsI16)(input->height - input->arial8_height * 2 - 9);
    if (input->ops->select_brush)
        input->ops->select_brush(input->context, input->hdc,
            input->brush_shadow);
    if (input->ops->draw_frame)
        input->ops->draw_frame(input->context, input->hdc, &body, 0);
    if (!input->part->present) {
        component_wrap_label(input, &body, COMPONENT_STRING_NO_PART, 0);
        return;
    }
    component_draw_header(input, &body);
    cost = component_cost(input);
    component_draw_costs(input, &body, cost);
    component_draw_detail(input, &body);
    if (input->ops->draw_frame)
        input->ops->draw_frame(input->context, input->hdc, &body,
            (StarsI16)(cost < 0 ? 1 : 0));
}
