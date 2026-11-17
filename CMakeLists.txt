# Bundled until the standalone touch-inertia module is published and SHA-pinned.
add_subdirectory(modules/zmk-input-processor-touch-inertia)
add_subdirectory(modules/zmk-input-processor-touch-swipe)
# Temporary wiring diagnostic (active only in the right USB-logging build).
target_sources(app PRIVATE ${CMAKE_CURRENT_LIST_DIR}/diag/pin_probe.c)
