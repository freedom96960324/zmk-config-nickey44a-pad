# Bundled until the standalone touch-inertia module is published and SHA-pinned.
add_subdirectory(modules/zmk-input-processor-touch-inertia)
add_subdirectory(modules/zmk-input-processor-touch-swipe)
# Wait for the right yuPad43 to power up on a cold start.
target_sources(app PRIVATE ${CMAKE_CURRENT_LIST_DIR}/src/yupad_power_up_delay.c)
