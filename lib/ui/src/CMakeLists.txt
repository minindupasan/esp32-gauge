SET(SOURCES ui_Screen1.c
    ui.c
    ui_comp_hook.c
    ui_helpers.c
    ui_events.c
    ui_img_ui_gauge_png.c
    ui_img_ui_fuel_tank_png.c
    ui_img_ui_ground_png.c
    ui_img_ui_motorbike_png.c
    ui_img_ui_charging_charging_off_png.c
    ui_img_ui_charging_charging_on_png.c
    ui_img_ui_lines_png.c
    fonts/ui_font_Font_14.c
    fonts/ui_font_Font_16.c
    fonts/ui_font_Font_18.c
    fonts/ui_font_Font_24.c
    fonts/ui_font_Font_22.c)

add_library(ui ${SOURCES})
