#include "flipfrid_scene_entrypoint.h"

string_t menu_items[4];

void flipfrid_scene_entrypoint_menu_callback(FlipFridState* context, uint32_t index) {
    switch(index) {
    case FlipFridAttackDefaultValues:
        context->attack = FlipFridAttackDefaultValues;
        context->current_scene = SceneAttack;
        string_set_str(context->attack_name, "Default Values");
        break;
    case FlipFridAttackBfCustomerId:
        context->attack = FlipFridAttackBfCustomerId;
        context->current_scene = SceneAttack;
        string_set_str(context->attack_name, "Bad Customer ID");
        break;
    case FlipFridAttackLoadFile:
        context->attack = FlipFridAttackLoadFile;
        context->current_scene = SceneSelectFile;
        string_set_str(context->attack_name, "Load File");
        break;
    case FlipFridAttackLoadFileCustomUids:
        context->attack = FlipFridAttackLoadFileCustomUids;
        context->current_scene = SceneLoadCustomUids;
        string_set_str(context->attack_name, "Load Custom UIDs");
        break;
    default:
        break;
    }
}

void flipfrid_scene_entrypoint_on_enter(FlipFridState* context) {
    // Clear the previous payload
    context->payload[0] = 0x00;
    context->payload[1] = 0x00;
    context->payload[2] = 0x00;
    context->payload[3] = 0x00;
    context->payload[4] = 0x00;

    context->menu_index = 0;
    for(uint32_t i = 0; i < 4; i++) {
        string_init(menu_items[i]);
    }

    string_set(menu_items[0], "Default Values");
    string_set(menu_items[1], "BF Customer ID");
    string_set(menu_items[2], "Load File");
    string_set(menu_items[3], "Load uids from file");
}

void flipfrid_scene_entrypoint_on_exit(FlipFridState* context) {
    UNUSED(context);
    for(uint32_t i = 0; i < 4; i++) {
        string_clear(menu_items[i]);
    }
}

void flipfrid_scene_entrypoint_on_tick(FlipFridState* context) {
    UNUSED(context);
}

void flipfrid_scene_entrypoint_on_event(FlipFridEvent event, FlipFridState* context) {
    if(event.evt_type == EventTypeKey) {
        if(event.input_type == InputTypeShort) {
            switch(event.key) {
            case InputKeyDown:
                if(context->menu_index < FlipFridAttackLoadFileCustomUids) {
                    context->menu_index++;
                }
                break;
            case InputKeyUp:
                if(context->menu_index > FlipFridAttackDefaultValues) {
                    context->menu_index--;
                }
                break;
            case InputKeyLeft:
            case InputKeyRight:
                break;
            case InputKeyOk:
                flipfrid_scene_entrypoint_menu_callback(context, context->menu_index);
                break;
            case InputKeyBack:
                context->is_running = false;
                break;
            }
        }
    }
}

void flipfrid_scene_entrypoint_on_draw(Canvas* canvas, FlipFridState* context) {
    canvas_clear(canvas);
    canvas_set_color(canvas, ColorBlack);

    // Title
    canvas_set_font(canvas, FontPrimary);
    canvas_draw_str_aligned(canvas, 64, 6, AlignCenter, AlignTop, "RFID Fuzzer");

    if(context->menu_index > FlipFridAttackDefaultValues) {
        canvas_set_font(canvas, FontSecondary);
        canvas_draw_str_aligned(
            canvas,
            64,
            24,
            AlignCenter,
            AlignTop,
            string_get_cstr(menu_items[context->menu_index - 1]));
    }

    canvas_set_font(canvas, FontPrimary);
    canvas_draw_str_aligned(
        canvas, 64, 36, AlignCenter, AlignTop, string_get_cstr(menu_items[context->menu_index]));

    if(context->menu_index < FlipFridAttackLoadFileCustomUids) {
        canvas_set_font(canvas, FontSecondary);
        canvas_draw_str_aligned(
            canvas,
            64,
            48,
            AlignCenter,
            AlignTop,
            string_get_cstr(menu_items[context->menu_index + 1]));
    }
}