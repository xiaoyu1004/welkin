# --- aim to adapt difference platform
configure_file(platform.h.in ${CMAKE_CURRENT_BINARY_DIR}/platform.h)

# --- CPU files
file(GLOB tmp *.cpp)
set(WELKIN_CPU_SRCS ${tmp})

# --- Send the lists to the parent scope.
set(WELKIN_CPU_SRCS ${WELKIN_CPU_SRCS} PARENT_SCOPE)